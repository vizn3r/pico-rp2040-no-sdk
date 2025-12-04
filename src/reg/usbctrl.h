//
// USB peripheral registers
//

#pragma once

#include <stdint.h>
#include <sys/types.h>

// Base registers
#define USB_CTRL_BASE 0x50100000
#define USB_DPSRAM_BASE 0x50100000
#define USB_REGS_BASE 0x50110000

// DPSRAM layout
#define USB_DPSRAM_SETUP_PACKET (*(volatile uint32_t *)(USB_DPSRAM_BASE + 0x0))

#define USB_DPSRAM_EP_IN_CTRL(EP_NUM)                                          \
  (*(volatile uint32_t *)(USB_DPSRAM_BASE + (EP_NUM * 8) + 0x08))
#define USB_DPSRAM_EP_OUT_CTRL(EP_NUM)                                         \
  (*(volatile uint32_t *)(USB_DPSRAM_BASE + (EP_NUM * 8) + 0xc))

#define USB_DPSRAM_EP_IN_BUFF_CTRL(EP_NUM)                                     \
  (*(volatile uint32_t *)(USB_DPSRAM_BASE + (EP_NUM * 8) + 0x80))
#define USB_DPSRAM_EP_OUT_BUFF_CTRL(EP_NUM)                                    \
  (*(volatile uint32_t *)(USB_DPSRAM_BASE + (EP_NUM * 8) + 0x84))

// USB registers bit definitions

// 0x00
typedef union {
  struct {
    uint32_t address : 7;
    uint32_t : 9;
    uint32_t endpoint : 4;
    uint32_t : 12;
  };
  uint32_t raw;
} usb_reg_addr_endp_t;

// 0x40
typedef union {
  struct {
    uint32_t controller_en : 1;
    uint32_t host_ndevice : 1;
    uint32_t : 30;
  };
  uint32_t raw;
} usb_reg_main_ctrl_t;

// 0x4c
typedef union {
  struct {
    uint32_t start_trans : 1;
    uint32_t send_setup : 1;
    uint32_t send_data : 1;
    uint32_t receive_data : 1;
    uint32_t stop_trans : 1;
    uint32_t : 1;
    uint32_t preamble_en : 1;
    uint32_t : 1;
    uint32_t sof_sync : 1;
    uint32_t sof_en : 1;
    uint32_t keep_alive_en : 1;
    uint32_t vbuen : 1;
    uint32_t resume : 1;
    uint32_t reset_bus : 1;
    uint32_t : 1;
    uint32_t pulldown_en : 1;
    uint32_t pullup_en : 1;
    uint32_t rpu_opt : 1;
    uint32_t transceiver_pd : 1;
    uint32_t : 5;
    uint32_t direct_dm : 1;
    uint32_t direct_dp : 1;
    uint32_t direct_en : 1;
    uint32_t ep0_int_nak : 1;
    uint32_t ep0_int_2buf : 1;
    uint32_t ep0_int_1buf : 1;
    uint32_t ep0_double_buf : 1;
    uint32_t ep0_int_stall : 1;
  };
  uint32_t raw;
} usb_reg_sie_ctrl_t;

// 0x50
typedef union {
  struct {
    uint32_t vbudetected : 1;
    uint32_t : 1;
    uint32_t line_state : 1;
    uint32_t suspended : 1;
    uint32_t : 3;
    uint32_t speed : 1;
    uint32_t vbuover_curr : 1;
    uint32_t resume : 1;
    uint32_t reserved : 1;
    uint32_t connected : 1;
    uint32_t setup_rec : 1;
    uint32_t trancomplete : 1;
    uint32_t bus_reset : 1;
    uint32_t : 4;
    uint32_t crc_error : 1;
    uint32_t bit_stuff_error : 1;
    uint32_t rx_overflow : 1;
    uint32_t rx_timeout : 1;
    uint32_t nak_rec : 1;
    uint32_t stall_rec : 1;
    uint32_t ack_rec : 1;
    uint32_t data_seq_error : 1;
  };
  uint32_t raw;
} usb_reg_sie_status_t;

// 0x54
typedef union {
  struct {
    uint32_t : 1;
    uint32_t int_ep_active : 15;
    uint32_t : 16;
  };
} usb_reg_int_ep_ctrl_t;

// 0x58
typedef union {
  struct {
    uint32_t ep0_in : 1;
    uint32_t ep0_out : 1;
    uint32_t ep1_in : 1;
    uint32_t ep1_out : 1;
    uint32_t ep2_in : 1;
    uint32_t ep2_out : 1;
    uint32_t ep3_in : 1;
    uint32_t ep3_out : 1;
    uint32_t ep4_in : 1;
    uint32_t ep4_out : 1;
    uint32_t ep5_in : 1;
    uint32_t ep5_out : 1;
    uint32_t ep6_in : 1;
    uint32_t ep6_out : 1;
    uint32_t ep7_in : 1;
    uint32_t ep7_out : 1;
    uint32_t ep8_in : 1;
    uint32_t ep8_out : 1;
    uint32_t ep9_in : 1;
    uint32_t ep9_out : 1;
    uint32_t ep10_in : 1;
    uint32_t ep10_out : 1;
    uint32_t ep11_in : 1;
    uint32_t ep11_out : 1;
    uint32_t ep12_in : 1;
    uint32_t ep12_out : 1;
    uint32_t ep13_in : 1;
    uint32_t ep13_out : 1;
    uint32_t ep14_in : 1;
    uint32_t ep14_out : 1;
    uint32_t ep15_in : 1;
    uint32_t ep15_out : 1;
  };
  uint32_t raw;
} usb_reg_buff_status_t;

// 0x74
typedef union {
  struct {
    uint32_t to_phy : 1;
    uint32_t to_extphy : 1;
    uint32_t to_digital_pad : 1;
    uint32_t softcon : 1;
    uint32_t : 28;
  };
  uint32_t raw;
} usb_reg_usb_muxing_t;

// 0x78
typedef union {
  struct {
    uint32_t vbuen : 1;
    uint32_t vbuen_override_en : 1;
    uint32_t vbudetect : 1;
    uint32_t vbudetect_override_en : 1;
    uint32_t overcurr_detect : 1;
    uint32_t overcurr_detect_en : 1;
    uint32_t : 26;
  };
  uint32_t raw;
} usb_reg_usb_pwr_t;

// 0x90
typedef union {
  struct {
    uint32_t host_conn_dis : 1;
    uint32_t host_resume : 1;
    uint32_t host_sof : 1;
    uint32_t trancomplete : 1;
    uint32_t buff_status : 1;
    uint32_t error_data_seq : 1;
    uint32_t error_rx_timeout : 1;
    uint32_t error_rx_overflow : 1;
    uint32_t error_bit_stuff : 1;
    uint32_t error_crc : 1;
    uint32_t stall : 1;
    uint32_t vbudetect : 1;
    uint32_t bus_reset : 1;
    uint32_t dev_conn_dis : 1;
    uint32_t dev_suspend : 1;
    uint32_t dev_resume_from_host : 1;
    uint32_t setup_req : 1;
    uint32_t dev_sof : 1;
    uint32_t abort_done : 1;
    uint32_t ep_stall_nak : 1;
    uint32_t : 12;
  };
  uint32_t raw;
} usb_reg_inte_t;

// 0x98
typedef union {
  struct {
    uint32_t host_conn_dis : 1;
    uint32_t host_resume : 1;
    uint32_t host_sof : 1;
    uint32_t trancomplete : 1;
    uint32_t buff_status : 1;
    uint32_t error_data_seq : 1;
    uint32_t error_rx_timeout : 1;
    uint32_t error_rx_overflow : 1;
    uint32_t error_bit_stuff : 1;
    uint32_t error_crc : 1;
    uint32_t stall : 1;
    uint32_t vbudetect : 1;
    uint32_t bus_reset : 1;
    uint32_t dev_conn_dis : 1;
    uint32_t dev_suspend : 1;
    uint32_t dev_resume_from_host : 1;
    uint32_t setup_req : 1;
    uint32_t dev_sof : 1;
    uint32_t abort_done : 1;
    uint32_t ep_stall_nak : 1;
    uint32_t : 12;
  };
  uint32_t raw;
} usb_reg_ints_t;

// USB registers
#define USB_REG_ADDR_ENDP                                                      \
  (*(volatile usb_reg_addr_endp_t *)(USB_REGS_BASE + 0x00))
#define USB_REG_ADDR_ENDP1 (*(volatile uint32_t *)(USB_REGS_BASE + 0x04))
#define USB_REG_ADDR_ENDP2 (*(volatile uint32_t *)(USB_REGS_BASE + 0x08))
#define USB_REG_ADDR_ENDP3 (*(volatile uint32_t *)(USB_REGS_BASE + 0x0c))
#define USB_REG_ADDR_ENDP4 (*(volatile uint32_t *)(USB_REGS_BASE + 0x10))
#define USB_REG_ADDR_ENDP5 (*(volatile uint32_t *)(USB_REGS_BASE + 0x14))
#define USB_REG_ADDR_ENDP6 (*(volatile uint32_t *)(USB_REGS_BASE + 0x18))
#define USB_REG_ADDR_ENDP7 (*(volatile uint32_t *)(USB_REGS_BASE + 0x1c))
#define USB_REG_ADDR_ENDP8 (*(volatile uint32_t *)(USB_REGS_BASE + 0x20))
#define USB_REG_ADDR_ENDP9 (*(volatile uint32_t *)(USB_REGS_BASE + 0x24))
#define USB_REG_ADDR_ENDP10 (*(volatile uint32_t *)(USB_REGS_BASE + 0x28))
#define USB_REG_ADDR_ENDP11 (*(volatile uint32_t *)(USB_REGS_BASE + 0x2c))
#define USB_REG_ADDR_ENDP12 (*(volatile uint32_t *)(USB_REGS_BASE + 0x30))
#define USB_REG_ADDR_ENDP13 (*(volatile uint32_t *)(USB_REGS_BASE + 0x34))
#define USB_REG_ADDR_ENDP14 (*(volatile uint32_t *)(USB_REGS_BASE + 0x38))
#define USB_REG_ADDR_ENDP15 (*(volatile uint32_t *)(USB_REGS_BASE + 0x3c))
#define USB_REG_MAIN_CTRL                                                      \
  (*(volatile usb_reg_main_ctrl_t *)(USB_REGS_BASE + 0x40))
#define USB_REG_SOF_WR (*(volatile uint32_t *)(USB_REGS_BASE + 0x44))
#define USB_REG_SOF_RD (*(volatile uint32_t *)(USB_REGS_BASE + 0x48))
#define USB_REG_SIE_CTRL                                                       \
  (*(volatile usb_reg_sie_ctrl_t *)(USB_REGS_BASE + 0x4c))
#define USB_REG_SIE_STATUS                                                     \
  (*(volatile usb_reg_sie_status_t *)(USB_REGS_BASE + 0x50))
#define USB_REG_INT_EP_CTRL                                                    \
  (*(volatile usb_reg_int_ep_ctrl_t *)(USB_REGS_BASE + 0x54))
#define USB_REG_BUFF_STATUS                                                    \
  (*(volatile usb_reg_buff_status_t *)(USB_REGS_BASE + 0x58))
#define USB_REG_BUFF_CPU_SHOULD_HANDLE                                         \
  (*(volatile uint32_t *)(USB_REGS_BASE + 0x5c))
#define USB_REG_EP_ABORT (*(volatile uint32_t *)(USB_REGS_BASE + 0x60))
#define USB_REG_EP_ABORT_DONE (*(volatile uint32_t *)(USB_REGS_BASE + 0x64))
#define USB_REG_EP_STALL_ARM (*(volatile uint32_t *)(USB_REGS_BASE + 0x68))
#define USB_REG_NAK_POLL (*(volatile uint32_t *)(USB_REGS_BASE + 0x6c))
#define USB_REG_EP_STATUSTALL_NAK (*(volatile uint32_t *)(USB_REGS_BASE + 0x70))
#define USB_REG_USB_MUXING                                                     \
  (*(volatile usb_reg_usb_muxing_t *)(USB_REGS_BASE + 0x74))
#define USB_REG_USB_PWR (*(volatile usb_reg_usb_pwr_t *)(USB_REGS_BASE + 0x78))
#define USB_REG_USBPHY_DIRECT (*(volatile uint32_t *)(USB_REGS_BASE + 0x7c))
#define USB_REG_USBPHY_DIRECT_OVERRIDE                                         \
  (*(volatile uint32_t *)(USB_REGS_BASE + 0x80))
#define USB_REG_USBPHY_TRIM (*(volatile uint32_t *)(USB_REGS_BASE + 0x84))
#define USB_REG_INTR (*(volatile uint32_t *)(USB_REGS_BASE + 0x8c))
#define USB_REG_INTE (*(volatile usb_reg_inte_t *)(USB_REGS_BASE + 0x90))
#define USB_REG_INTF (*(volatile uint32_t *)(USB_REGS_BASE + 0x94))
#define USB_REG_INTS (*(volatile usb_reg_ints_t *)(USB_REGS_BASE + 0x98))
