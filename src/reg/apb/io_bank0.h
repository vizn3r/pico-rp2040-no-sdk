//
// IO_BANK0_BASE
//

#pragma once

#include <stdint.h>

#define IO_BANK0_BASE 0x40014000

#define GPIO_STAT(gpio) *(volatile uint32_t *)(IO_BANK0_BASE + (gpio) * 8)
#define GPIO_CTRL(gpio) *(volatile uint32_t *)(IO_BANK0_BASE + (gpio) * 8 + 4)

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
