#include <hal/gpio.h>
#include <kernel/kernel.h>
#include <serial/usb.h>

#include <stdint.h>

static int strlen(const char *s) {
  int len = 0;
  while (s[len])
    len++;
  return len;
}

int kernel_main(void) {
  gpio_enable_led_b();

  gpio_led_blink_fast_b(3);

  usb_init_b();
  const char *data = "Hello world!";
  while (1) {
    for (volatile int i = 0; i < 1e6; i++)
      ;
    usb_cdc_send((const uint8_t *)data, strlen(data));
  }

  return 0;
}
