#pragma once

#include <stdint.h>

void gpio_enable_led();

void gpio_led_blink(uint16_t n);
void gpio_led_blink_fast(uint16_t n);
