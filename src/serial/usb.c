#include <hal/gpio.h>
#include <reg/apb/clock.h>
#include <reg/apb/pll.h>
#include <reg/apb/resets.h>
#include <reg/apb/xosc.h>
#include <reg/ppb.h>
#include <reg/sio.h>
#include <reg/usbctrl.h>
#include <serial/usb.h>

#include <stdint.h>
#include <sys/types.h>

uint8_t usb_cdc_recv_buff[64];
uint16_t usb_cdc_recv_buff_len;

static uint8_t ep0_data_toggle = 0;
static uint8_t ep2_in_data_toggle = 0;
static volatile uint8_t configured = 0;
static volatile uint8_t pending_address = 0;

void usb_init_b(void) {
  gpio_led_blink_b(1);
  __asm volatile("cpsie i");

  // Enable XOSC
  XOSC_REG_CTRL.raw = 0xAA0;
  XOSC_REG_STATUS.raw = 47;
  while (!(XOSC_REG_STARTUP.raw & 0x80000000))
    ;
  gpio_led_blink_b(1);

  // Configure PLL_USB
  RESETS_RESET_CLR(RESETS_RESET_OFFSET_PLL_USB);
  while (!RESETS_RESET_DONE_OK(RESETS_RESET_OFFSET_PLL_USB))
    ;
  gpio_led_blink_b(1);

  PLL_USB_REG_FBDIV_INT.raw = (1 << 5) | (1 << 8);
  PLL_USB_REG_CS.raw = 1;
  PLL_USB_REG_PWR.raw = 40;
  PLL_USB_REG_FBDIV_INT.raw = 0;
  while (!(PLL_USB_REG_CS.raw & 0x80000000))
    ;
  gpio_led_blink_b(1);
  PLL_USB_REG_PRIM.raw = (5 << 16) | (2 << 12);

  // Configure CLK_USB from PLL_USB (48MHz)
  CLOCK_REG_CLK_USB_DIV.raw = (1 << 8);
  CLOCK_REG_CLK_USB_CTRL.raw = (1 << 11);

  // Bring USB controller out of reset
  RESETS_RESET_CLR(RESETS_RESET_OFFSET_USBCTRL);
  while (!RESETS_RESET_DONE_OK(RESETS_RESET_OFFSET_USBCTRL))
    ;
  gpio_led_blink_b(1);

  // Clear DPSRAM
  for (int i = 0; i < 4096; i += 4) {
    *(volatile uint32_t *)(USBCTRL_DPSRAM_BASE + i) = 0;
  }

  USBCTRL_REG_MAIN_CTRL.raw = 0;
  USBCTRL_REG_USB_MUXING.to_phy = 1;
  USBCTRL_REG_USB_MUXING.softcon = 1;

  USBCTRL_REG_USB_PWR.vbus_detect = 1;
  USBCTRL_REG_USB_PWR.vbus_detect_override_en = 1;

  USBCTRL_REG_MAIN_CTRL.controller_en = 1;

  while (!USBCTRL_REG_SIE_STATUS.vbus_detected)
    ;

  USBCTRL_REG_INTE.setup_req = 1;
  USBCTRL_REG_INTE.buff_status = 1;
  USBCTRL_REG_INTE.bus_reset = 1;

  USBCTRL_REG_SIE_CTRL.pullup_en = 1;

  USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(0) = 64 | (1 << 10);

  PPB_REG_NVIC_ISER.raw |= (1 << 5);

  gpio_led_blink_fast_b(3);
}

void usb_irq_handle(void) {
  SIO_GPIO_OUT_SET(LED_GPIO);
  if (USBCTRL_REG_INTS.setup_req) {
    volatile uint8_t *setup = (volatile uint8_t *)USBCTRL_DPSRAM_SETUP_PACKET;

    usb_setup_packet_t *packet = (usb_setup_packet_t *)setup;

    uint8_t desc_type;
    uint8_t desc_index;

    uint16_t len;

    const uint8_t *str_desc;
    uint16_t str_len;

    switch (packet->bRequest) {
    case 0x05: // SET_ADDRESS
      pending_address = packet->wValue & 0x7F;
      ep0_data_toggle = 1;
      USBCTRL_DPSRAM_EP_IN_BUFF_CTRL(0) = (1 << 13) | (1 << 15) | (1 << 10);
      break;
    case 0x06: // GET_DESCRIPTOR
      desc_type = (packet->wValue >> 8) & 0xFF;
      desc_index = packet->wValue & 0xFF;

      switch (desc_type) {
      case 0x01: // Device descriptor
        len = sizeof(device_descriptor);
        if (len > packet->wLength)
          len = packet->wLength;

        volatile uint8_t *ep0_in_buf =
            (volatile uint8_t *)(USBCTRL_DPSRAM_BASE + 0x100);
        for (int i = 0; i < len; i++) {
          ep0_in_buf[i] = device_descriptor[i];
        }

        ep0_data_toggle = 1;
        USBCTRL_DPSRAM_EP_IN_BUFF_CTRL(0) =
            len | (1 << 13) | (1 << 15) | (1 << 10);
        break;
      case 0x02: // Configuration descriptor
        len = sizeof(config_descriptor);
        if (len > packet->wLength)
          len = packet->wLength;

        ep0_in_buf = (volatile uint8_t *)(USBCTRL_DPSRAM_BASE + 0x100);

        for (int i = 0; i < len; i++) {
          ep0_in_buf[i] = config_descriptor[i];
        }

        USBCTRL_DPSRAM_EP_IN_BUFF_CTRL(0) = len | (1 << 15) | (1 << 31);

        break;
      case 0x03: // String descriptor
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

        if (str_len > packet->wLength)
          str_len = packet->wLength;

        ep0_in_buf = (volatile uint8_t *)(USBCTRL_DPSRAM_BASE + 0x100);

        for (int i = 0; i < str_len; i++) {
          ep0_in_buf[i] = str_desc[i];
        }

        USBCTRL_DPSRAM_EP_IN_BUFF_CTRL(0) = str_len | (1 << 15) | (1 << 31);
        break;
      }
      break;
    case 0x09: // SET_CONFIGURATION
      configured = 1;
      USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(2) = 64 | (1 << 10);
      ep0_data_toggle = 1;
      USBCTRL_DPSRAM_EP_IN_BUFF_CTRL(0) = (1 << 13) | (1 << 15) | (1 << 10);
      break;
    default:
      break;
    }
    USBCTRL_REG_SIE_STATUS.setup_rec = 1;
  }

  if (USBCTRL_REG_INTS.buff_status) {
    uint32_t buffers = USBCTRL_REG_BUFF_STATUS.raw;

    USBCTRL_REG_BUFF_STATUS.raw = buffers;

    if (buffers & (1 << 0)) {
      if (pending_address) {
        USBCTRL_REG_ADDR_ENDP.address = pending_address;
        pending_address = 0;
      }
    }
    if (buffers & (1 << 2)) {
      // interrupt done sending
    }
    if (buffers & (1 << 4)) {
      usb_cdc_recv_buff_len = usb_cdc_recv(usb_cdc_recv_buff, 64);
    }
    if (buffers & (1 << 5)) {
      // bulk in finished sending
    }
  }

  if (USBCTRL_REG_INTS.bus_reset) {
    USBCTRL_REG_ADDR_ENDP.address = 0;
    configured = 0;
    pending_address = 0;
    ep0_data_toggle = 0;
    ep2_in_data_toggle = 0;
    USBCTRL_REG_SIE_STATUS.bus_reset = 1;
  }
}

void usb_cdc_send(const uint8_t *data, uint16_t len) {
  if (!configured || len > 64)
    return;
  if (len > 64)
    len = 64;

  volatile uint8_t *ep2_in_buf =
      (volatile uint8_t *)(USBCTRL_DPSRAM_BASE + 0x1C0);

  for (int i = 0; i < len; i++) {
    ep2_in_buf[i] = data[i];
  }

  uint32_t buf_ctrl = len | (1 << 15) | (1 << 10);
  if (ep2_in_data_toggle) {
    buf_ctrl |= (1 << 13);
  }
  ep2_in_data_toggle ^= 1;

  USBCTRL_DPSRAM_EP_IN_BUFF_CTRL(2) = buf_ctrl;
}

uint16_t usb_cdc_recv(uint8_t *data, uint16_t max_len) {
  volatile uint8_t *ep2_out_buf =
      (volatile uint8_t *)(USBCTRL_DPSRAM_BASE + 0x180);

  uint16_t len = USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(2) & 0x3FF;
  if (len > max_len)
    len = max_len;

  for (int i = 0; i < len; i++) {
    data[i] = ep2_out_buf[i];
  }

  USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(2) = 64 | (1 << 15) | (1 << 31);

  return len;
}
