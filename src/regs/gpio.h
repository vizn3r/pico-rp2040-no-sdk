//
// GPIO peripheral registers
//

#pragma once

#include <stdint.h>

//
// RESETS_BASE
//

#define RESETS_BASE(offset) (0x4000c000 + (offset))

#define RESETS_RESET_RST 0x1
#define RESETS_WDSEL_RST 0x0
#define RESETS_DONE_RST 0x0

#define RESETS_RESET(peripheral) *(volatile uint32_t *)RESETS_BASE(0x0)

#define RESETS_RESET_CLR(peripheral)                                           \
  RESETS_RESET(peripheral) &= ~(RESETS_RESET_RST << (peripheral))

#define RESETS_WDSEL(watchdog) *(volatile uint32_t *)RESETS_BASE(0x4)

#define RESETS_WDSEL_CLR(watchdog)                                             \
  RESETS_WDSEL(watchdog) &= ~(RESETS_WDSEL_RST << (watchdog))

#define RESETS_RESET_DONE(done) *(volatile uint32_t *)RESETS_BASE(0x8)

#define RESETS_RESET_DONE_OK(done) (RESETS_RESET_DONE(done) & (1 << (done)))

// Reset bits
#define RESETS_RESET_OFFSET_USBCTRL 24
#define RESETS_RESET_OFFSET_UART1 23
#define RESETS_RESET_OFFSET_UART0 22
#define RESETS_RESET_OFFSET_TIMER 21
#define RESETS_RESET_OFFSET_TBMAN 20
#define RESETS_RESET_OFFSET_SYSINFO 19
#define RESETS_RESET_OFFSET_SYSCFG 18
#define RESETS_RESET_OFFSET_SPI1 17
#define RESETS_RESET_OFFSET_SPI0 16
#define RESETS_RESET_OFFSET_RTC 15
#define RESETS_RESET_OFFSET_PWM 14
#define RESETS_RESET_OFFSET_PLL_USB 13
#define RESETS_RESET_OFFSET_PLL_SYS 12
#define RESETS_RESET_OFFSET_PIO1 11
#define RESETS_RESET_OFFSET_PIO0 10
#define RESETS_RESET_OFFSET_PADS_QSPI 9
#define RESETS_RESET_OFFSET_PADS_BANK0 8
#define RESETS_RESET_OFFSET_JTAG 7
#define RESETS_RESET_OFFSET_IO_QSPI 6
#define RESETS_RESET_OFFSET_IO_BANK0 5
#define RESETS_RESET_OFFSET_I2C1 4
#define RESETS_RESET_OFFSET_I2C0 3
#define RESETS_RESET_OFFSET_DMA 2
#define RESETS_RESET_OFFSET_BUSCTRL 1
#define RESETS_RESET_OFFSET_ADC 0

//
// IO_BANK0_BASE
//
//
#define IO_BANK0_BASE(offset) (0x40014000 + (offset))

#define GPIO_STAT(gpio) *(volatile uint32_t *)IO_BANK0_BASE((gpio) * 8)
#define GPIO_CTRL(gpio) *(volatile uint32_t *)IO_BANK0_BASE(((gpio) * 8) + 4)

#define GPIO_CTRL_OEOVER 12
#define OEOVER_NORMAL 0x0
#define OEOVER_INVERT 0x1
#define OEOVER_DISABLE 0x2
#define OEOVER_ENABLE 0x3

#define GPIO_CTRL_OUTOVER 8
#define OUTOVER_NORMAL 0x0
#define OUTOVER_INVERT 0x1
#define OUTOVER_LOW 0x2
#define OUTOVER_HIGH 0x3

//
// SIO_BASE
//

#define SIO_BASE(offset) (0xd0000000 + (offset))

#define SIO_GPIO_OUT_SET(gpio)                                                 \
  *(volatile uint32_t *)SIO_BASE(0x14) = (1 << (gpio))

#define SIO_GPIO_OUT_CLR(gpio)                                                 \
  *(volatile uint32_t *)SIO_BASE(0x18) = (1 << (gpio))

#define SIO_GPIO_OE_SET(gpio)                                                  \
  *(volatile uint32_t *)SIO_BASE(0x024) = (1 << (gpio))

#define LED_GPIO 25
