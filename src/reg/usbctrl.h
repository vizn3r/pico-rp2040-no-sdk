//
// USBCTRL_BASE
// USBCTRL_DPSRAM_BASE
// USBCTRL_REGS_BASE
//

#pragma once

#include <stdint.h>
#include <sys/types.h>

// Base registers
#define USBCTRL_BASE 0x50100000
#define USBCTRL_DPSRAM_BASE 0x50100000
#define USBCTRL_REGS_BASE 0x50110000

typedef union {
  struct {
    uint32_t : 6;
    uint32_t buf_addr : 10;
    uint32_t intr_on_nak : 1;
    uint32_t intr_on_stall : 1;
    uint32_t : 8;
    uint32_t ep_type : 2;
    uint32_t intr_per_double_buf : 1;
    uint32_t int_per_tx_buf : 1;
    uint32_t double_buf : 1;
    uint32_t enable : 1;
  };
  uint32_t raw;
} usbctrl_dpsram_ep_ctrl_t;

typedef union {
  struct {
    uint32_t len0 : 10;
    uint32_t available0 : 1;
    uint32_t stall : 1;
    uint32_t reset : 1;
    uint32_t pid0 : 1;
    uint32_t last0 : 1;
    uint32_t full0 : 1;
    uint32_t len1 : 10;
    uint32_t available1 : 1;
    uint32_t double_buf_offset : 2;
    uint32_t pid1 : 1;
    uint32_t last1 : 1;
    uint32_t full1 : 1;
  };
  uint32_t raw;
} usbctrl_dpsram_buf_ctrl_t;

// DPSRAM layout
#define USBCTRL_DPSRAM_SETUP_PACKET                                            \
  (*(volatile uint32_t *)(USBCTRL_DPSRAM_BASE + 0x0))

#define USBCTRL_DPSRAM_EP_IN_CTRL(EP_NUM)                                      \
  (*(volatile usbctrl_dpsram_ep_ctrl_t *)(USBCTRL_DPSRAM_BASE + (EP_NUM) * 8 + \
                                          0x08))
#define USBCTRL_DPSRAM_EP_OUT_CTRL(EP_NUM)                                     \
  (*(volatile usbctrl_dpsram_ep_ctrl_t *)(USBCTRL_DPSRAM_BASE + (EP_NUM) * 8 + \
                                          0xc))

#define USBCTRL_DPSRAM_EP_IN_BUFF_CTRL(EP_NUM)                                 \
  (*(volatile usbctrl_dpsram_buf_ctrl_t *)(USBCTRL_DPSRAM_BASE +               \
                                           (EP_NUM) * 8 + 0x80))
#define USBCTRL_DPSRAM_EP_OUT_BUFF_CTRL(EP_NUM)                                \
  (*(volatile usbctrl_dpsram_buf_ctrl_t *)(USBCTRL_DPSRAM_BASE +               \
                                           (EP_NUM) * 8 + 0x84))

// USB registers bit definitions

// 0x00 - 0x3c
typedef union {
  struct {
    uint32_t address : 7;
    uint32_t : 9;
    uint32_t endpoint : 4;
    uint32_t : 12;
  };
  uint32_t raw;
} usbctrl_addr_endp_t;

// 0x40
typedef union {
  struct {
    uint32_t controller_en : 1;
    uint32_t host_ndevice : 1;
    uint32_t : 30;
  };
  uint32_t raw;
} usbctrl_main_ctrl_t;

typedef union {
  struct {
    uint32_t : 11;
    uint32_t : 21;
  };
  uint32_t raw;
} usbctrl_sof_wr_t;

typedef union {
  struct {
    uint32_t : 11;
    uint32_t : 21;
  };
  uint32_t raw;
} usbctrl_sof_rd_t;
;

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
    uint32_t vbus_en : 1;
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
} usbctrl_sie_ctrl_t;

// 0x50
typedef union {
  struct {
    uint32_t vbus_detected : 1;
    uint32_t : 1;
    uint32_t line_state : 2;
    uint32_t suspended : 1;
    uint32_t : 3;
    uint32_t speed : 2;
    uint32_t vbus_over_curr : 1;
    uint32_t resume : 1;
    uint32_t : 4;
    uint32_t connected : 1;
    uint32_t setup_rec : 1;
    uint32_t trans_complete : 1;
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
} usbctrl_sie_status_t;

// 0x54
typedef union {
  struct {
    uint32_t : 1;
    uint32_t int_ep_active : 15;
    uint32_t : 16;
  };
} usbctrl_int_ep_ctrl_t;

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
} usbctrl_buff_status_t;

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
} usbctrl_buff_cpu_should_handle_t;

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
} usbctrl_ep_abort_t;

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
} usbctrl_ep_abort_done_t;

typedef union {
  struct {
    uint32_t ep0_in : 1;
    uint32_t ep0_out : 1;
    uint32_t : 30;
  };
  uint32_t raw;
} usbctrl_ep_stall_arm_t;

typedef union {
  struct {
    uint32_t delay_ls : 10;
    uint32_t : 6;
    uint32_t delay_fs : 10;
    uint32_t : 6;
  };
  uint32_t raw;
} usbctrl_nak_poll_t;

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
} usbctrl_ep_status_nak_t;

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
} usbctrl_usb_muxing_t;

// 0x78
typedef union {
  struct {
    uint32_t vbus_en : 1;
    uint32_t vbus_en_override_en : 1;
    uint32_t vbus_detect : 1;
    uint32_t vbus_detect_override_en : 1;
    uint32_t overcurr_detect : 1;
    uint32_t overcurr_detect_en : 1;
    uint32_t : 26;
  };
  uint32_t raw;
} usbctrl_usb_pwr_t;

typedef union {
  struct {
    uint32_t dp_pullup_hisel : 1;
    uint32_t dp_pullup_en : 1;
    uint32_t dp_pulldn_en : 1;
    uint32_t : 1;
    uint32_t dm_pullup_hisel : 1;
    uint32_t dm_pullup_en : 1;
    uint32_t dm_pulldn_en : 1;
    uint32_t : 1;
    uint32_t tx_dp_oe : 1;
    uint32_t tx_dm_oe : 1;
    uint32_t tx_dp : 1;
    uint32_t tx_dm : 1;
    uint32_t rx_pd : 1;
    uint32_t tx_pd : 1;
    uint32_t tx_fsslew : 1;
    uint32_t tx_diffmode : 1;
    uint32_t rx_dd : 1;
    uint32_t rx_dp : 1;
    uint32_t rx_dm : 1;
    uint32_t dp_ovcn : 1;
    uint32_t dm_ovcn : 1;
    uint32_t dp_ovv : 1;
    uint32_t dm_ovv : 1;
    uint32_t : 9;
  };
  uint32_t raw;
} usbctrl_usbphy_direct_t;

typedef union {
  struct {
    uint32_t dp_pullup_hisel_override_en : 1;
    uint32_t dm_pullup_hisel_override_en : 1;
    uint32_t dp_pullup_en_override_en : 1;
    uint32_t dp_pulldn_en_override_en : 1;
    uint32_t dm_pulldn_en_override_en : 1;
    uint32_t tx_dp_oe_override_en : 1;
    uint32_t tx_dm_oe_override_en : 1;
    uint32_t tx_dp_override_en : 1;
    uint32_t tx_dm_override_en : 1;
    uint32_t rx_pd_override_en : 1;
    uint32_t tx_pd_override_en : 1;
    uint32_t tx_fsslew_override_en : 1;
    uint32_t dm_pullup_override_en : 1;
    uint32_t : 2;
    uint32_t tx_diffmode_override_en : 1;
    uint32_t : 16;
  };
  uint32_t raw;
} usbctrl_usbphy_direct_override_t;

typedef union {
  struct {
    uint32_t dp_pulldn_trim : 5;
    uint32_t : 3;
    uint32_t dm_pulldn_trim : 5;
    uint32_t : 19;
  };
  uint32_t raw;
} usbctrl_usbphy_trim_t;

typedef union {
  struct {
    uint32_t host_conn_dis : 1;
    uint32_t host_resume : 1;
    uint32_t host_sof : 1;
    uint32_t trans_complete : 1;
    uint32_t buff_status : 1;
    uint32_t error_data_seq : 1;
    uint32_t error_rx_timeout : 1;
    uint32_t error_rx_overflow : 1;
    uint32_t error_bit_stuff : 1;
    uint32_t error_crc : 1;
    uint32_t stall : 1;
    uint32_t vbus_detect : 1;
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
} usbctrl_intr_t;

typedef union {
  struct {
    uint32_t host_conn_dis : 1;
    uint32_t host_resume : 1;
    uint32_t host_sof : 1;
    uint32_t trans_complete : 1;
    uint32_t buff_status : 1;
    uint32_t error_data_seq : 1;
    uint32_t error_rx_timeout : 1;
    uint32_t error_rx_overflow : 1;
    uint32_t error_bit_stuff : 1;
    uint32_t error_crc : 1;
    uint32_t stall : 1;
    uint32_t vbus_detect : 1;
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
} usbctrl_inte_t;

typedef union {
  struct {
    uint32_t host_conn_dis : 1;
    uint32_t host_resume : 1;
    uint32_t host_sof : 1;
    uint32_t trans_complete : 1;
    uint32_t buff_status : 1;
    uint32_t error_data_seq : 1;
    uint32_t error_rx_timeout : 1;
    uint32_t error_rx_overflow : 1;
    uint32_t error_bit_stuff : 1;
    uint32_t error_crc : 1;
    uint32_t stall : 1;
    uint32_t vbus_detect : 1;
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
} usbctrl_intf_t;

typedef union {
  struct {
    uint32_t host_conn_dis : 1;
    uint32_t host_resume : 1;
    uint32_t host_sof : 1;
    uint32_t trans_complete : 1;
    uint32_t buff_status : 1;
    uint32_t error_data_seq : 1;
    uint32_t error_rx_timeout : 1;
    uint32_t error_rx_overflow : 1;
    uint32_t error_bit_stuff : 1;
    uint32_t error_crc : 1;
    uint32_t stall : 1;
    uint32_t vbus_detect : 1;
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
} usbctrl_ints_t;

// USB registers
#define USBCTRL_ADDR_ENDP                                                      \
  (*(volatile usbctrl_addr_endp_t *)(USBCTRL_REGS_BASE + 0x00))
#define USBCTRL_ADDR_ENDP1                                                     \
  (*(volatile usbctrl_addr_endp_t *)(USBCTRL_REGS_BASE + 0x04))
#define USBCTRL_ADDR_ENDP2                                                     \
  (*(volatile usbctrl_addr_endp_t *)(USBCTRL_REGS_BASE + 0x08))
#define USBCTRL_ADDR_ENDP3                                                     \
  (*(volatile usbctrl_addr_endp_t *)(USBCTRL_REGS_BASE + 0x0c))
#define USBCTRL_ADDR_ENDP4                                                     \
  (*(volatile usbctrl_addr_endp_t *)(USBCTRL_REGS_BASE + 0x10))
#define USBCTRL_ADDR_ENDP5                                                     \
  (*(volatile usbctrl_addr_endp_t *)(USBCTRL_REGS_BASE + 0x14))
#define USBCTRL_ADDR_ENDP6                                                     \
  (*(volatile usbctrl_addr_endp_t *)(USBCTRL_REGS_BASE + 0x18))
#define USBCTRL_ADDR_ENDP7                                                     \
  (*(volatile usbctrl_addr_endp_t *)(USBCTRL_REGS_BASE + 0x1c))
#define USBCTRL_ADDR_ENDP8                                                     \
  (*(volatile usbctrl_addr_endp_t *)(USBCTRL_REGS_BASE + 0x20))
#define USBCTRL_ADDR_ENDP9                                                     \
  (*(volatile usbctrl_addr_endp_t *)(USBCTRL_REGS_BASE + 0x24))
#define USBCTRL_ADDR_ENDP10                                                    \
  (*(volatile usbctrl_addr_endp_t *)(USBCTRL_REGS_BASE + 0x28))
#define USBCTRL_ADDR_ENDP11                                                    \
  (*(volatile usbctrl_addr_endp_t *)(USBCTRL_REGS_BASE + 0x2c))
#define USBCTRL_ADDR_ENDP12                                                    \
  (*(volatile usbctrl_addr_endp_t *)(USBCTRL_REGS_BASE + 0x30))
#define USBCTRL_ADDR_ENDP13                                                    \
  (*(volatile usbctrl_addr_endp_t *)(USBCTRL_REGS_BASE + 0x34))
#define USBCTRL_ADDR_ENDP14                                                    \
  (*(volatile usbctrl_addr_endp_t *)(USBCTRL_REGS_BASE + 0x38))
#define USBCTRL_ADDR_ENDP15                                                    \
  (*(volatile usbctrl_addr_endp_t *)(USBCTRL_REGS_BASE + 0x3c))
#define USBCTRL_MAIN_CTRL                                                      \
  (*(volatile usbctrl_main_ctrl_t *)(USBCTRL_REGS_BASE + 0x40))
#define USBCTRL_SOF_WR                                                         \
  (*(volatile usbctrl_sof_wr_t *)(USBCTRL_REGS_BASE + 0x44))
#define USBCTRL_SOF_RD                                                         \
  (*(volatile usbctrl_sof_rd_t *)(USBCTRL_REGS_BASE + 0x48))
#define USBCTRL_SIE_CTRL                                                       \
  (*(volatile usbctrl_sie_ctrl_t *)(USBCTRL_REGS_BASE + 0x4c))
#define USBCTRL_SIE_STATUS                                                     \
  (*(volatile usbctrl_sie_status_t *)(USBCTRL_REGS_BASE + 0x50))
#define USBCTRL_INT_EP_CTRL                                                    \
  (*(volatile usbctrl_int_ep_ctrl_t *)(USBCTRL_REGS_BASE + 0x54))
#define USBCTRL_BUFF_STATUS                                                    \
  (*(volatile usbctrl_buff_status_t *)(USBCTRL_REGS_BASE + 0x58))
#define USBCTRL_BUFF_CPU_SHOULD_HANDLE                                         \
  (*(volatile uint32_t *)(USBCTRL_REGS_BASE + 0x5c))
#define USBCTRL_EP_ABORT                                                       \
  (*(volatile usbctrl_ep_abort_t *)(USBCTRL_REGS_BASE + 0x60))
#define USBCTRL_EP_ABORT_DONE                                                  \
  (*(volatile usbctrl_ep_abort_done_t *)(USBCTRL_REGS_BASE + 0x64))
#define USBCTRL_EP_STALL_ARM                                                   \
  (*(volatile usbctrl_ep_stall_arm_t *)(USBCTRL_REGS_BASE + 0x68))
#define USBCTRL_NAK_POLL                                                       \
  (*(volatile usbctrl_nak_poll_t *)(USBCTRL_REGS_BASE + 0x6c))
#define USBCTRL_EP_STATUSTALL_NAK                                              \
  (*(volatile uint32_t *)(USBCTRL_REGS_BASE + 0x70))
#define USBCTRL_USB_MUXING                                                     \
  (*(volatile usbctrl_usb_muxing_t *)(USBCTRL_REGS_BASE + 0x74))
#define USBCTRL_USB_PWR                                                        \
  (*(volatile usbctrl_usb_pwr_t *)(USBCTRL_REGS_BASE + 0x78))
#define USBCTRL_USBPHY_DIRECT                                                  \
  (*(volatile usbctrl_usbphy_direct_t *)(USBCTRL_REGS_BASE + 0x7c))
#define USBCTRL_USBPHY_DIRECT_OVERRIDE                                         \
  (*(volatile uint32_t *)(USBCTRL_REGS_BASE + 0x80))
#define USBCTRL_USBPHY_TRIM                                                    \
  (*(volatile usbctrl_usbphy_trim_t *)(USBCTRL_REGS_BASE + 0x84))
#define USBCTRL_INTR (*(volatile usbctrl_intr_t *)(USBCTRL_REGS_BASE + 0x8c))
#define USBCTRL_INTE (*(volatile usbctrl_inte_t *)(USBCTRL_REGS_BASE + 0x90))
#define USBCTRL_INTF (*(volatile usbctrl_intf_t *)(USBCTRL_REGS_BASE + 0x94))
#define USBCTRL_INTS (*(volatile usbctrl_ints_t *)(USBCTRL_REGS_BASE + 0x98))
