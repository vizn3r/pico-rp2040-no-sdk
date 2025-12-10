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
  // Enable XOSC
  XOSC_CTRL.enable = XOSC_CTRL_ENABLE;
  XOSC_STARTUP.delay = 47;
  XOSC_CTRL.freq_range = XOSC_CTRL_FREQ_1_15MHz;
  while (!XOSC_STATUS.stable) {
  }

  // Reset PLL_USB
  RESETS_RESET_CLR(RESETS_RESET_OFFSET_PLL_USB);
  while (!RESETS_RESET_DONE_OK(RESETS_RESET_OFFSET_PLL_USB)) {
  }

  // Configure PLL_USB for 48MHz
  PLL_USB_CS.refdiv = 1;
  PLL_USB_FBDIV_INT.ctrl = 40;
  PLL_USB_PRIM.postdiv1 = 5;
  PLL_USB_PRIM.postdiv2 = 2;
  PLL_USB_PWR.pd = 0;
  PLL_USB_PWR.vcopd = 0;
  while (!PLL_USB_CS.lock) {
  }
  PLL_USB_PWR.postdivpd = 0;

  // Configure USB clock
  CLOCK_CLK_USB_DIV.integer = 1;
  CLOCK_CLK_USB_CTRL.enable = 1;

  // Bring USB controller out of reset
  RESETS_RESET_CLR(RESETS_RESET_OFFSET_USBCTRL);
  while (!RESETS_RESET_DONE_OK(RESETS_RESET_OFFSET_USBCTRL)) {
  }

  // Clear DPSRAM
  for (uint32_t i = 0; i < 4096; i += 4) {
    *(volatile uint32_t *)(USBCTRL_DPSRAM_BASE + i) = 0;
  }

  // Configure endpoints in DPSRAM
  // EP1 IN (Interrupt for CDC)
  USBCTRL_DPSRAM_EP_IN_CTRL(1).enable = 1;
  USBCTRL_DPSRAM_EP_IN_CTRL(1).ep_type = USBCTRL_DPSRAM_EP_TYPE_INTR;
  USBCTRL_DPSRAM_EP_IN_CTRL(1).buf_addr = 0x140 / 64;

  // EP2 OUT (Bulk Data)
  USBCTRL_DPSRAM_EP_OUT_CTRL(2).enable = 1;
  USBCTRL_DPSRAM_EP_OUT_CTRL(2).ep_type = USBCTRL_DPSRAM_EP_TYPE_BULK;
  USBCTRL_DPSRAM_EP_OUT_CTRL(2).buf_addr = 0x180 / 64;

  // EP2 IN (Bulk Data)
  USBCTRL_DPSRAM_EP_IN_CTRL(2).enable = 1;
  USBCTRL_DPSRAM_EP_IN_CTRL(2).ep_type = USBCTRL_DPSRAM_EP_TYPE_BULK;
  USBCTRL_DPSRAM_EP_IN_CTRL(2).buf_addr = 0x1C0 / 64;

  // Setup USB controller
  USBCTRL_MAIN_CTRL.host_ndevice = 0;
  USBCTRL_MAIN_CTRL.controller_en = 1;
  USBCTRL_USB_MUXING.to_phy = 1;
  USBCTRL_USB_MUXING.softcon = 1;
  USBCTRL_USB_PWR.vbus_detect = 1;
  USBCTRL_USB_PWR.vbus_detect_override_en = 1;

  // Wait for VBUS
  while (!USBCTRL_SIE_STATUS.vbus_detected) {
  }

  // Setup interrupts
  USBCTRL_INTE.bus_reset = 1;
  USBCTRL_INTE.setup_req = 1;
  USBCTRL_INTE.buff_status = 1;

  // Arm EP0 OUT for first setup packet
  // USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(0).len0 = 64;
  // USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(0).available0 = 1;
  // USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(0).pid0 = 1;
  // USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(0).full0 = 0;

  USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(0).raw =
      64 | (1 << 10); // len=64, available=1, pid0=0, full=0

  // Enable USB interrupt in NVIC
  PPB_NVIC_ISER.raw |= (1 << 5);

  // Enable global interrupts
  __asm volatile("cpsie i");

  // Enable pull-up resistor (connect to host)
  USBCTRL_SIE_CTRL.pullup_en = 1;
}

static void ep0_in_send(const uint8_t *data, uint16_t len) {
  volatile uint8_t *ep0_in_buf = USBCTRL_DPSRAM_EP_IN_BUF(0);

  for (int i = 0; i < len; i++) {
    ep0_in_buf[i] = data[i];
  }

  USBCTRL_DPSRAM_EP_IN_BUFF_CTRL(0).len0 = len;
  USBCTRL_DPSRAM_EP_IN_BUFF_CTRL(0).pid0 = ep0_data_toggle;
  USBCTRL_DPSRAM_EP_IN_BUFF_CTRL(0).full0 = 1;
  USBCTRL_DPSRAM_EP_IN_BUFF_CTRL(0).available0 = 1;

  ep0_data_toggle ^= 1;
}

void _usb_irq_handle(void) {
  // Bus reset interrupt
  if (USBCTRL_INTS.bus_reset) {
    USBCTRL_ADDR_ENDP.address = 0;
    configured = 0;
    pending_address = 0;
    ep0_data_toggle = 0;
    ep2_in_data_toggle = 0;

    // Re-arm EP0 OUT
    USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(0).len0 = 64;
    USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(0).available0 = 1;
    USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(0).pid0 = 0;
    USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(0).full0 = 0;

    USBCTRL_SIE_STATUS.bus_reset = 1;
  }

  // Setup request interrupt
  if (USBCTRL_INTS.setup_req) {
    usbctrl_dpsram_setup_t setup = USBCTRL_DPSRAM_SETUP;

    ep0_data_toggle = 1;

    // Standard device requests
    if (setup.bmRequestType.recipient == USBCTRL_SETUP_RECIPIENT_DEVICE &&
        setup.bmRequestType.type == USBCTRL_SETUP_TYPE_STANDARD) {

      switch (setup.bRequest) {
      case USBCTRL_SETUP_REQ_GET_STATUS: {
        uint8_t s[2] = {0, 0};
        ep0_in_send(s, 2);
        break;
      }

      case USBCTRL_SETUP_REQ_SET_ADDRESS: {
        pending_address = setup.wValue.raw & 0x7F;
        ep0_in_send(NULL, 0);
        break;
      }

      case USBCTRL_SETUP_REQ_GET_DESCRIPTOR: {
        const uint8_t *desc_ptr = NULL;
        uint16_t desc_len = 0;

        switch (setup.wValue.descriptor_type) {
        case USBCTRL_SETUP_DESC_TYPE_DEVICE:
          desc_ptr = device_descriptor;
          desc_len = sizeof(device_descriptor);
          break;
        case USBCTRL_SETUP_DESC_TYPE_CONFIGURATION:
          desc_ptr = config_descriptor;
          desc_len = sizeof(config_descriptor);
          break;
        case USBCTRL_SETUP_DESC_TYPE_STRING:
          switch (setup.wValue.descriptor_index) {
          case 0:
            desc_ptr = string_descriptor_0;
            desc_len = sizeof(string_descriptor_0);
            break;
          case 1:
            desc_ptr = string_descriptor_1;
            desc_len = sizeof(string_descriptor_1);
            break;
          case 2:
            desc_ptr = string_descriptor_2;
            desc_len = sizeof(string_descriptor_2);
            break;
          }
          break;
        }

        if (desc_ptr && desc_len > 0) {
          uint16_t send_len = desc_len;
          if (send_len > setup.wLength)
            send_len = setup.wLength;
          ep0_in_send(desc_ptr, send_len);
        } else {
          ep0_in_send(NULL, 0);
        }
        break;
      }

      case USBCTRL_SETUP_REQ_GET_CONFIGURATION: {
        uint8_t b[1] = {configured};
        ep0_in_send(b, 1);
        break;
      }

      case USBCTRL_SETUP_REQ_SET_CONFIGURATION: {
        configured = (setup.wValue.raw & 0xFF) ? 1 : 0;
        if (configured) {
          USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(2).len0 = 64;
          USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(2).available0 = 1;
          USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(2).pid0 = 0;
          USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(2).full0 = 0;
        }
        ep0_in_send(NULL, 0);
        break;
      }

      default:
        ep0_in_send(NULL, 0);
        break;
      }
    }
    // Interface requests (CDC ACM)
    else if (setup.bmRequestType.recipient ==
                 USBCTRL_SETUP_RECIPIENT_INTERFACE &&
             setup.bmRequestType.type == USBCTRL_SETUP_TYPE_CLASS) {

      switch (setup.bRequest) {
      case USBCTRL_SETUP_REQ_CDC_SET_LINE_CODING:
        ep0_in_send(NULL, 0);
        break;

      case USBCTRL_SETUP_REQ_CDC_GET_LINE_CODING: {
        uint8_t buf[7] = {0x00, 0xC2, 0x01, 0x00, 0x00, 0x00, 0x08};
        ep0_in_send(buf, 7);
        break;
      }

      case USBCTRL_SETUP_REQ_CDC_SET_CONTROL_LINE_STATE:
        ep0_in_send(NULL, 0);
        break;

      default:
        ep0_in_send(NULL, 0);
        break;
      }
    } else {
      ep0_in_send(NULL, 0);
    }

    USBCTRL_SIE_STATUS.setup_rec = 1;
  }

  // Buffer status interrupt
  if (USBCTRL_INTS.buff_status) {
    usbctrl_buff_status_t buffers = USBCTRL_BUFF_STATUS;
    USBCTRL_BUFF_STATUS.raw = buffers.raw;

    if (buffers.ep0_out) {
      if (pending_address) {
        USBCTRL_ADDR_ENDP.address = pending_address;
        pending_address = 0;
      }
      USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(0).len0 = 64;
      USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(0).available0 = 1;
      USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(0).pid0 = 0;
      USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(0).full0 = 0;
    }

    if (buffers.ep2_out) {
      volatile uint8_t *ep2_out_buf = USBCTRL_DPSRAM_EP_OUT_BUF(2);
      uint16_t len = USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(2).len0;

      if (len > 64)
        len = 64;
      for (int i = 0; i < len; i++) {
        usb_cdc_recv_buff[i] = ep2_out_buf[i];
      }
      usb_cdc_recv_buff_len = len;

      USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(2).len0 = 64;
      USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(2).available0 = 1;
      USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(2).pid0 = 0;
      USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(2).full0 = 0;
    }
  }
}

void usb_cdc_send(const uint8_t *data, uint16_t len) {
  if (!configured)
    return;
  if (len > 64)
    len = 64;

  while (USBCTRL_DPSRAM_EP_IN_BUFF_CTRL(2).full0) {
  }

  volatile uint8_t *ep2_in_buf = USBCTRL_DPSRAM_EP_IN_BUF(2);
  for (int i = 0; i < len; i++) {
    ep2_in_buf[i] = data[i];
  }

  USBCTRL_DPSRAM_EP_IN_BUFF_CTRL(2).raw = 0;
  USBCTRL_DPSRAM_EP_IN_BUFF_CTRL(2).available0 = 1;
  USBCTRL_DPSRAM_EP_IN_BUFF_CTRL(2).len0 = len;
  USBCTRL_DPSRAM_EP_IN_BUFF_CTRL(2).pid0 = ep2_in_data_toggle;
  USBCTRL_DPSRAM_EP_IN_BUFF_CTRL(2).full0 = 1;

  ep2_in_data_toggle ^= 1;
}

uint16_t usb_cdc_recv(uint8_t *data, uint16_t max_len) {
  if (usb_cdc_recv_buff_len > max_len) {
    uint16_t copy_len = max_len;
    for (int i = 0; i < copy_len; i++) {
      data[i] = usb_cdc_recv_buff[i];
    }
    for (int i = copy_len; i < usb_cdc_recv_buff_len; i++) {
      usb_cdc_recv_buff[i - copy_len] = usb_cdc_recv_buff[i];
    }
    usb_cdc_recv_buff_len -= copy_len;
    return copy_len;
  } else {
    uint16_t copy_len = usb_cdc_recv_buff_len;
    for (int i = 0; i < copy_len; i++) {
      data[i] = usb_cdc_recv_buff[i];
    }
    usb_cdc_recv_buff_len = 0;
    return copy_len;
  }
}
