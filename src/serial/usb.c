#include "../regs/usb.h"
#include "../regs/gpio.h"
#include "usb.h"
#include <stdint.h>
#include <sys/types.h>

uint8_t s_usb_cdc_recv_buff[64];
uint16_t s_usb_cdc_recv_buff_len;

void s_usb_init_blocking(void) {
  HW_RESETS_RESET_CLR(HW_RESETS_RESET_OFFSET_USBCTRL);
  while (!HW_RESETS_RESET_DONE_OK(HW_RESETS_RESET_OFFSET_USBCTRL))
    ;

  // Clear DPSRAM
  for (int i = 0; i < 4096; i++) {
    *(volatile uint32_t *)(S_USB_DPSRAM_BASE + i) = 0;
  }

  S_USB_REG_MAIN_CTRL.raw = 0;
  S_USB_REG_USB_MUXING.to_phy = 1;

  S_USB_REG_USB_PWR.vbus_detect = 1;
  S_USB_REG_USB_PWR.vbus_detect_override_en = 1;

  S_USB_REG_MAIN_CTRL.controller_en = 1;

  // Blocking until USB not connected
  while (!S_USB_REG_SIE_STATUS.vbus_detected)
    ;

  S_USB_REG_INTE.setup_req = 1;
  S_USB_REG_INTE.buff_status = 1;
  S_USB_REG_INTE.bus_reset = 1;

  S_USB_REG_SIE_CTRL.pullup_en = 1;
}

void s_usb_irq_handle(void) {
  if (S_USB_REG_INTS.setup_req) {
    volatile uint8_t *setup = (volatile uint8_t *)S_USB_DPSRAM_SETUP_PACKET;

    s_usb_setup_packet_t *packet = (s_usb_setup_packet_t *)setup;

    uint8_t desc_type;
    uint8_t desc_index;

    volatile uint8_t *ep0_in_buf;
    uint16_t len;

    const uint8_t *str_desc;
    uint16_t str_len;

    S_USB_REG_SIE_STATUS.setup_rec = 1;
    switch (packet->bRequest) {
    case 0x05: // SES_ADDRESS
      S_USB_REG_ADDR_ENDP.address = packet->wValue & 0x7F;

      S_USB_DPSRAM_EP_IN_BUFF_CTRL(0) = 0x80000000;
      break;
    case 0x06: // SET_DESCRIPTOR
      desc_type = (packet->wValue >> 8) & 0xFF;
      desc_index = packet->wValue & 0xFF;

      switch (desc_type) {
      case 0x01:
        len = sizeof(device_descriptor);
        if (len > packet->wLenght)
          len = packet->wLenght;

        ep0_in_buf = (volatile uint8_t *)(S_USB_DPSRAM_BASE + 0x100);

        for (int i = 0; i < len; i++) {
          ep0_in_buf[i] = device_descriptor[i];
        }

        // full 15 bit, avaliable in bit 31
        S_USB_DPSRAM_EP_IN_BUFF_CTRL(0) = len | (1 << 15) | (1 << 31);
        break;
      case 0x02:
        len = sizeof(config_descriptor);
        if (len > packet->wLenght)
          len = packet->wLenght;

        ep0_in_buf = (volatile uint8_t *)(S_USB_DPSRAM_BASE + 0x100);

        for (int i = 0; i < len; i++) {
          ep0_in_buf[i] = config_descriptor[i];
        }

        // full 15 bit, avaliable in bit 31
        S_USB_DPSRAM_EP_IN_BUFF_CTRL(0) = len | (1 << 15) | (1 << 31);

        break;
      case 0x03:
        switch (desc_index) {
        case 0:
          str_desc = string_descriptor_0;
          str_len = sizeof(string_descriptor_0);
          break;
        case 1:
          str_desc = string_descriptor_1;
          str_len = sizeof(string_descriptor_1);
          break;
        case 2:
          str_desc = string_descriptor_2;
          str_len = sizeof(string_descriptor_2);
          break;
        default:
          return;
        }

        if (str_len > packet->wLenght)
          str_len = packet->wLenght;

        ep0_in_buf = (volatile uint8_t *)(S_USB_DPSRAM_BASE + 0x100);

        for (int i = 0; i < str_len; i++) {
          ep0_in_buf[i] = str_desc[i];
        }

        S_USB_DPSRAM_EP_IN_BUFF_CTRL(0) = str_len | (1 << 15) | (1 << 31);
        break;
      }
      break;
    case 0x09: // SET_CONFIGURATION
      S_USB_DPSRAM_EP_IN_CTRL(1) = 0x0;
      S_USB_DPSRAM_EP_OUT_CTRL(2) = 0x0;
      S_USB_DPSRAM_EP_IN_CTRL(2) = 0x0;
      S_USB_DPSRAM_EP_IN_BUFF_CTRL(0) = 0x80000000;
      break;
    default:
      break;
    }
  }

  if (S_USB_REG_INTS.buff_status) {
    uint32_t buffers = S_USB_REG_BUFF_STATUS.raw;

    S_USB_REG_BUFF_STATUS.raw = buffers;

    if (buffers & (1 << 2)) {
      // interrupt done sending
    }
    if (buffers & (1 << 4)) {
      // receive
      s_usb_cdc_recv_buff_len = s_usb_cdc_recv(s_usb_cdc_recv_buff, 64);
    }
    if (buffers & (1 << 5)) {
      // bulk in finished sending
    }
  }

  if (S_USB_REG_INTS.bus_reset) {
    S_USB_REG_ADDR_ENDP.address = 0;
    S_USB_REG_SIE_STATUS.bus_reset = 1;
  }
}

void s_usb_cdc_send(const uint8_t *data, uint16_t len) {
  if (len > 64)
    len = 64;

  volatile uint8_t *ep2_in_buf =
      (volatile uint8_t *)(S_USB_DPSRAM_BASE + 0x180);

  for (int i = 0; i < len; i++) {
    ep2_in_buf[i] = data[i];
  }

  S_USB_DPSRAM_EP_IN_BUFF_CTRL(2) = len | (1 << 15) | (1 << 31);
}

uint16_t s_usb_cdc_recv(uint8_t *data, uint16_t max_len) {
  volatile uint8_t *ep2_out_buf =
      (volatile uint8_t *)(S_USB_DPSRAM_BASE + 0x140);

  uint16_t len = S_USB_DPSRAM_EP_OUT_BUFF_CTRL(2) & 0x3FF;
  if (len > max_len)
    len = max_len;

  for (int i = 0; i < len; i++) {
    data[i] = ep2_out_buf[i];
  }

  S_USB_DPSRAM_EP_OUT_BUFF_CTRL(2) = 64 | (1 << 15) | (1 << 31);

  return len;
}
