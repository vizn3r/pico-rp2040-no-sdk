//
// SIO_BASE
//

#pragma once

#include <stdint.h>

#define SIO_BASE 0xd0000000

#define SIO_GPIO_OUT_SET(gpio)                                                 \
  *(volatile uint32_t *)(SIO_BASE + 0x14) = (1 << (gpio))

#define SIO_GPIO_OUT_CLR(gpio)                                                 \
  *(volatile uint32_t *)(SIO_BASE + 0x18) = (1 << (gpio))

#define SIO_GPIO_OE_SET(gpio)                                                  \
  *(volatile uint32_t *)(SIO_BASE + 0x024) = (1 << (gpio))

#define LED_GPIO 25
