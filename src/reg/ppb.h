//
// PPB_BASE
//

#pragma once

#include <stdint.h>

#define PPB_BASE 0xe0000000

typedef union {
  struct {
    uint32_t enable : 1;
    uint32_t tickint : 1;
    uint32_t clksource : 1;
    uint32_t : 13;
    uint32_t countflag : 1;
    uint32_t : 15;
  };
  uint32_t raw;
} ppb_reg_syst_csr_t;

typedef union {
  struct {
    uint32_t reload : 24;
    uint32_t : 8;
  };
  uint32_t raw;
} ppb_reg_syst_rvr_t;

typedef union {
  struct {
    uint32_t current : 24;
    uint32_t : 8;
  };
  uint32_t raw;
} ppb_reg_syst_cvr_t;

typedef union {
  struct {
    uint32_t tenms : 24;
    uint32_t : 6;
    uint32_t skew : 1;
    uint32_t noref : 1;
  };
  uint32_t raw;
} ppb_reg_syst_calib_t;

typedef union {
  struct {
    uint32_t setena : 32;
  };
  uint32_t raw;
} ppb_reg_nvic_iser_t;

typedef union {
  struct {
    uint32_t clrena : 32;
  };
  uint32_t raw;
} ppb_reg_nvic_icer_t;

typedef union {
  struct {
    uint32_t setpend : 32;
  };
  uint32_t raw;
} ppb_reg_nvic_ispr_t;

typedef union {
  struct {
    uint32_t clrpend : 32;
  };
  uint32_t raw;
} ppb_reg_nvic_icpr_t;

typedef union {
  struct {
    uint32_t : 6;
    uint32_t ip_0 : 2;
    uint32_t : 6;
    uint32_t ip_1 : 2;
    uint32_t : 6;
    uint32_t ip_2 : 2;
    uint32_t : 6;
    uint32_t ip_3 : 2;
  };
  uint32_t raw;
} ppb_reg_nvic_ipr0_t;

typedef union {
  struct {
    uint32_t : 6;
    uint32_t ip_4 : 2;
    uint32_t : 6;
    uint32_t ip_5 : 2;
    uint32_t : 6;
    uint32_t ip_6 : 2;
    uint32_t : 6;
    uint32_t ip_7 : 2;
  };
  uint32_t raw;
} ppb_reg_nvic_ipr1_t;

typedef union {
  struct {
    uint32_t : 6;
    uint32_t ip_8 : 2;
    uint32_t : 6;
    uint32_t ip_9 : 2;
    uint32_t : 6;
    uint32_t ip_10 : 2;
    uint32_t : 6;
    uint32_t ip_11 : 2;
  };
  uint32_t raw;
} ppb_reg_nvic_ipr2_t;

typedef union {
  struct {
    uint32_t : 6;
    uint32_t ip_12 : 2;
    uint32_t : 6;
    uint32_t ip_13 : 2;
    uint32_t : 6;
    uint32_t ip_14 : 2;
    uint32_t : 6;
    uint32_t ip_15 : 2;
  };
  uint32_t raw;
} ppb_reg_nvic_ipr3_t;

typedef union {
  struct {
    uint32_t : 6;
    uint32_t ip_16 : 2;
    uint32_t : 6;
    uint32_t ip_17 : 2;
    uint32_t : 6;
    uint32_t ip_18 : 2;
    uint32_t : 6;
    uint32_t ip_19 : 2;
  };
  uint32_t raw;
} ppb_reg_nvic_ipr4_t;

typedef union {
  struct {
    uint32_t : 6;
    uint32_t ip_20 : 2;
    uint32_t : 6;
    uint32_t ip_21 : 2;
    uint32_t : 6;
    uint32_t ip_22 : 2;
    uint32_t : 6;
    uint32_t ip_23 : 2;
  };
  uint32_t raw;
} ppb_reg_nvic_ipr5_t;

typedef union {
  struct {
    uint32_t : 6;
    uint32_t ip_24 : 2;
    uint32_t : 6;
    uint32_t ip_25 : 2;
    uint32_t : 6;
    uint32_t ip_26 : 2;
    uint32_t : 6;
    uint32_t ip_27 : 2;
  };
  uint32_t raw;
} ppb_reg_nvic_ipr6_t;

typedef union {
  struct {
    uint32_t : 6;
    uint32_t ip_28 : 2;
    uint32_t : 6;
    uint32_t ip_29 : 2;
    uint32_t : 6;
    uint32_t ip_30 : 2;
    uint32_t : 6;
    uint32_t ip_31 : 2;
  };
  uint32_t raw;
} ppb_reg_nvic_ipr7_t;

typedef union {
  struct {
    uint32_t revision : 4;
    uint32_t partno : 12;
    uint32_t architecture : 4;
    uint32_t variant : 4;
    uint32_t implementer : 8;
  };
  uint32_t raw;
} ppb_reg_cpuid_t;

typedef union {
  struct {
    uint32_t vectactive : 9;
    uint32_t : 3;
    uint32_t vectpending : 9;
    uint32_t : 1;
    uint32_t isrpending : 1;
    uint32_t isrpreempt : 1;
    uint32_t : 1;
    uint32_t pendstclr : 1;
    uint32_t pendstset : 1;
    uint32_t pendsvclr : 1;
    uint32_t pendsvset : 1;
    uint32_t : 2;
    uint32_t nmipendset : 1;
  };
  uint32_t raw;
} ppb_reg_icsr_t;

typedef union {
  struct {
    uint32_t : 8;
    uint32_t tbloff : 24;
  };
  uint32_t raw;
} ppb_reg_vtor_t;

typedef union {
  struct {
    uint32_t : 1;
    uint32_t vectclractive : 1;
    uint32_t sysresetreq : 1;
    uint32_t : 12;
    uint32_t endianess : 1;
    uint32_t vectkey : 16;
  };
  uint32_t raw;
} ppb_reg_aircr_t;

typedef union {
  struct {
    uint32_t : 1;
    uint32_t sleeponexit : 1;
    uint32_t sleepdeep : 1;
    uint32_t : 1;
    uint32_t sevonpend : 1;
    uint32_t : 27;
  };
  uint32_t raw;
} ppb_reg_scr_t;

typedef union {
  struct {
    uint32_t : 3;
    uint32_t unalign_trp : 1;
    uint32_t : 5;
    uint32_t stkalign : 1;
    uint32_t : 22;
  };
  uint32_t raw;
} ppb_reg_ccr_t;

typedef union {
  struct {
    uint32_t : 30;
    uint32_t pri_11 : 2;
  };
  uint32_t raw;
} ppb_reg_shpr2_t;

typedef union {
  struct {
    uint32_t : 22;
    uint32_t pri_14 : 2;
    uint32_t : 6;
    uint32_t pri_15 : 2;
  };
  uint32_t raw;
} ppb_reg_shpr3_t;

typedef union {
  struct {
    uint32_t : 15;
    uint32_t svcallpended : 1;
    uint32_t : 16;
  };
  uint32_t raw;
} ppb_reg_shcsr_t;

typedef union {
  struct {
    uint32_t separate : 1;
    uint32_t : 7;
    uint32_t dregion : 8;
    uint32_t iregion : 8;
    uint32_t : 8;
  };
  uint32_t raw;
} ppb_reg_mpu_type_t;

typedef union {
  struct {
    uint32_t enable : 1;
    uint32_t hfnmiena : 1;
    uint32_t privdefena : 1;
    uint32_t : 29;
  };
  uint32_t raw;
} ppb_reg_mpu_ctrl_t;

typedef union {
  struct {
    uint32_t region : 4;
    uint32_t : 28;
  };
  uint32_t raw;
} ppb_reg_mpu_rnr_t;

typedef union {
  struct {
    uint32_t region : 4;
    uint32_t valid : 1;
    uint32_t : 3;
    uint32_t addr : 24;
  };
  uint32_t raw;
} ppb_reg_mpu_rbar_t;

typedef union {
  struct {
    uint32_t enable : 1;
    uint32_t size : 5;
    uint32_t : 2;
    uint32_t srd : 8;
    uint32_t attrs : 16;
  };
  uint32_t raw;
} ppb_reg_mpu_rasr_t;

#define PPB_REG_SYST_CSR (*(volatile ppb_reg_syst_csr_t *)(PPB_BASE + 0xe010))
#define PPB_REG_SYST_RVR (*(volatile ppb_reg_syst_rvr_t *)(PPB_BASE + 0xe014))
#define PPB_REG_SYST_CVR (*(volatile ppb_reg_syst_cvr_t *)(PPB_BASE + 0xe018))
#define PPB_REG_SYST_CALIB                                                     \
  (*(volatile ppb_reg_syst_calib_t *)(PPB_BASE + 0xe01c))
#define PPB_REG_NVIC_ISER (*(volatile ppb_reg_nvic_iser_t *)(PPB_BASE + 0xe100))
#define PPB_REG_NVIC_ICER (*(volatile ppb_reg_nvic_icer_t *)(PPB_BASE + 0xe180))
#define PPB_REG_NVIC_ISPR (*(volatile ppb_reg_nvic_ispr_t *)(PPB_BASE + 0xe200))
#define PPB_REG_NVIC_ICPR (*(volatile ppb_reg_nvic_icpr_t *)(PPB_BASE + 0xe280))
#define PPB_REG_NVIC_IPR0 (*(volatile ppb_reg_nvic_ipr0_t *)(PPB_BASE + 0xe400))
#define PPB_REG_NVIC_IPR1 (*(volatile ppb_reg_nvic_ipr1_t *)(PPB_BASE + 0xe404))
#define PPB_REG_NVIC_IPR2 (*(volatile ppb_reg_nvic_ipr2_t *)(PPB_BASE + 0xe408))
#define PPB_REG_NVIC_IPR3 (*(volatile ppb_reg_nvic_ipr3_t *)(PPB_BASE + 0xe40c))
#define PPB_REG_NVIC_IPR4 (*(volatile ppb_reg_nvic_ipr4_t *)(PPB_BASE + 0xe410))
#define PPB_REG_NVIC_IPR5 (*(volatile ppb_reg_nvic_ipr5_t *)(PPB_BASE + 0xe414))
#define PPB_REG_NVIC_IPR6 (*(volatile ppb_reg_nvic_ipr6_t *)(PPB_BASE + 0xe418))
#define PPB_REG_NVIC_IPR7 (*(volatile ppb_reg_nvic_ipr7_t *)(PPB_BASE + 0xe41c))
#define PPB_REG_CPUID (*(volatile ppb_reg_cpuid_t *)(PPB_BASE + 0xed00))
#define PPB_REG_ICSR (*(volatile ppb_reg_icsr_t *)(PPB_BASE + 0xed04))
#define PPB_REG_VTOR (*(volatile ppb_reg_vtor_t *)(PPB_BASE + 0xed08))
#define PPB_REG_AIRCR (*(volatile ppb_reg_aircr_t *)(PPB_BASE + 0xed0c))
#define PPB_REG_SCR (*(volatile ppb_reg_scr_t *)(PPB_BASE + 0xed10))
#define PPB_REG_CCR (*(volatile ppb_reg_ccr_t *)(PPB_BASE + 0xed14))
#define PPB_REG_SHPR2 (*(volatile ppb_reg_shpr2_t *)(PPB_BASE + 0xed1c))
#define PPB_REG_SHPR3 (*(volatile ppb_reg_shpr3_t *)(PPB_BASE + 0xed20))
#define PPB_REG_SHCSR (*(volatile ppb_reg_shcsr_t *)(PPB_BASE + 0xed24))
#define PPB_REG_MPU_TYPE (*(volatile ppb_reg_mpu_type_t *)(PPB_BASE + 0xed90))
#define PPB_REG_MPU_CTRL (*(volatile ppb_reg_mpu_ctrl_t *)(PPB_BASE + 0xed94))
#define PPB_REG_MPU_RNR (*(volatile ppb_reg_mpu_rnr_t *)(PPB_BASE + 0xed98))
#define PPB_REG_MPU_RBAR (*(volatile ppb_reg_mpu_rbar_t *)(PPB_BASE + 0xed9c))
#define PPB_REG_MPU_RASR (*(volatile ppb_reg_mpu_rasr_t *)(PPB_BASE + 0xeda0))
