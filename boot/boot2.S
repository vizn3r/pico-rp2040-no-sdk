.syntax unified
.cpu cortex-m0plus
.thumb

.section .boot2, "ax"

// enable QSPI mode for the SSI flash
//#define SPI_FAST

#define SSI_BASE      0x18000000
#define PPB_BASE      0xE0000000
#define APP_BASE      0x10000100
#define VTOR          0xED08

// SSI_CTRL0 values
#ifdef SPI_FAST
#define SPI_FRF (0x2 << 21) // 0x2 > QUAD SPI
#else
#define SPI_FRF (0x0 << 21)
#endif
#define DFS_32 (31 << 16)
#define TMOD (0x3 << 8)

#define SSI_CTRL0 (TMOD | DFS_32 | SPI_FRF)

// SSI_SPI_CTRL0 values
#ifdef SPI_FAST
#define XIP_CMD (0xA0 << 24) // for QSPI
#define TRANS_TYPE (0x2 << 0) // for QSPI
#define INST_L (0x0 << 8)
#define ADDR_L (8 << 2)
#define WAIT_CYCLES (4 << 11)
#else
#define XIP_CMD (0x03 << 24)
#define TRANS_TYPE (0x0 << 0)
#define INST_L (0x2 << 8)
#define ADDR_L (6 << 2)
#define WAIT_CYCLES (0 << 11)
#endif

#define SSI_SPI_CTRL0 (TRANS_TYPE | ADDR_L | INST_L | WAIT_CYCLES | XIP_CMD)

#define QSPI_PAD 0x40020000

.global _stage2_boot
.type _stage2_boot,%function
.thumb_func
_stage2_boot:
  bl startup

  ldr r0, =APP_BASE
  ldr r1, =(PPB_BASE + VTOR)
  str r0, [r1]

  ldmia r0, {r0, r1}
  msr msp, r0
  bx r1

startup:

ssi_pads:
  ldr r0, =QSPI_PAD
  movs r1, #(2 << 4 | 1) // set DRIVE to 8MA and SLEWFAST to fast
  str r1, [r0, #0x04]

  ldr r2, [r0, #0x08] // Take the current SD0 config
  movs r1, #2 // bitmask for Schmitt
  bics r2, r1
  str r2, [r0, #0x08]
  str r2, [r0, #0x0c]
  str r2, [r0, #0x10]
  str r2, [r0, #0x14]

  // Disable SSI
  ldr r0, =SSI_BASE
  movs r1, #0
  str r1, [r0, #0x08]

  // Setup BAUD
  movs r1, #4
  str r1, [r0, #0x14]

#ifdef SPI_FAST

  // RX sample delay
  movs r1, #1
  movs r2, #0xf0
  str r1, [r0, r2]

flash_setup:
  // DFS_32 and TMOD
  ldr r1, =(7 << 16 | 0 << 8) // 8 bits per data frame, RX and TX mode
  str r1, [r0, #0]

  // Enable SSI
  movs r1, #1
  str r1, [r0, #0x08]

  movs r3, #0x35 // status register 2
  str r3, [r0, #0x60] // write to SSI data register
  str r3, [r0, #0x60] // dummy byte

  bl wait_for_ssi
    
  ldr r3, [r0, #0x60] // discard first byte
  ldr r3, [r0, #0x60]

  movs r2, #0x02 // value of enabled QSPI
  cmp r3, r2
  beq skip_flash_setup

  // configure flash into QSPI
  movs r1, #0x06 // write enable
  str r1, [r0, #0x60]

  bl wait_for_ssi
  
  ldr r1, [r0, #0x60] // discard rx

  // write status
  movs r1, #0x01 // status register 1
  str r1, [r0, #0x60]
  movs r2, #0
  movs r3, #0x02
  str r2, [r0, #0x60] // register 1 data
  str r3, [r0, #0x60] // register 2 data

  bl wait_for_ssi

  ldr r1, [r0, #0x60]
  ldr r1, [r0, #0x60]
  ldr r1, [r0, #0x60]

  // poll status register for completion

  1:
    movs r1, #0x05 // read status register 1
    str r1, [r0, #0x60] // write to SSI data register
    str r1, [r0, #0x60] // dummy byte

    bl wait_for_ssi
      
    ldr r3, [r0, #0x60] // discard first byte
    ldr r3, [r0, #0x60]
  
    movs r2, #1
    tst r3, r2
    bne 1b

skip_flash_setup:
  // Disable SSI
  movs r1, #0
  str r1, [r0, #0x08]

  // configure SSI params
  ldr r1, =(31 << 16 | 0x3 << 8 | 0x2 << 21) // DFS_32, EEPROM read, Quad
  str r1, [r0, #0]

  ldr r1, =(4 << 11 | 8 << 2 | 0x2 << 8 | 0x1 << 0) // 8bit cmd, 32bit addr+mode, 4 wait cycles, trans type 1
  movs r2, #0xf4
  str r1, [r0, r2]

  movs r1, #0
  str r1, [r0, #0x04] // NDF 0

  // Enable SSI
  movs r1, #1
  str r1, [r0, #0x08]

  // dummy data
  ldr r1, =0xEB
  str r1, [r0, #0x60]
  ldr r1, =0xA0000000 // wrap for continuous read
  str r1, [r0, #0x60]

  bl wait_for_ssi

  ldr r1, [r0, #0x60]

  // Disable SSI
  movs r1, #0
  str r1, [r0, #0x08]

#endif

ssi_config:

  // Setup CTRL0
  ldr r1, =SSI_CTRL0
  str r1, [r0, #0]

  // Setup SPI_CTRL0
  ldr r1, =SSI_SPI_CTRL0
  movs r2, #0xF4
  str r1, [r0, r2]

  // Enable SSI
  movs r1, #1
  str r1, [r0, #0x08]

  bx lr

dead:
    wfi
    b dead

wait_for_ssi:
  // wait until SSI ready
  1:
    ldr r1, [r0, #0x28] // read SSI status register
    movs r3, #0b0100 // TFE bit
    tst r1, r3
    beq 1b
    movs r3, #0b1 // BUSY bit
    tst r1, r3
    bne 1b

  bx lr

.ltorg

// Padding to 252 bytes
.space (252 - (. - _stage2_boot))

.word 0x00000000

.end
