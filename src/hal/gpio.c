#include <hal/gpio.h>
#include <reg/apb/io_bank0.h>
#include <reg/apb/resets.h>
#include <reg/sio.h>
#include <stdint.h>

void gpio_enable_led_b(void) {
  RESETS_RESET_CLR(RESETS_RESET_OFFSET_IO_BANK0);
  RESETS_RESET_CLR(RESETS_RESET_OFFSET_PADS_BANK0);

  // blocking part
  while (!RESETS_RESET_DONE_OK(RESETS_RESET_OFFSET_IO_BANK0))
    ;
  while (!RESETS_RESET_DONE_OK(RESETS_RESET_OFFSET_PADS_BANK0))
    ;

  GPIO_CTRL(LED_GPIO) = 5;
  SIO_GPIO_OE_SET(LED_GPIO);
}

void gpio_led_blink_b(uint16_t n) {
  for (volatile uint16_t i = 0; i < n; i++) {
    SIO_GPIO_OUT_SET(LED_GPIO);
    for (volatile uint32_t i = 0; i < 500000; i++)
      ;

    SIO_GPIO_OUT_CLR(LED_GPIO);
    for (volatile uint32_t i = 0; i < 500000; i++)
      ;
  }
}

void gpio_led_blink_fast_b(uint16_t n) {
  for (volatile uint16_t i = 0; i < n; i++) {
    SIO_GPIO_OUT_SET(LED_GPIO);
    for (volatile uint32_t i = 0; i < 30000; i++)
      ;

    SIO_GPIO_OUT_CLR(LED_GPIO);
    for (volatile uint32_t i = 0; i < 30000; i++)
      ;
  }
}
