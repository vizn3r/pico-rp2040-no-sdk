#include "kernel.h"
#include "../regs/gpio.h"
#include "../serial/usb.h"
#include <stdint.h>

static int strlen(const char *s) {
  int len = 0;
  while (s[len])
    len++;
  return len;
}

int kernel_main(void) {
  HW_RESETS_RESET_CLR(HW_RESETS_RESET_OFFSET_IO_BANK0);
  HW_RESETS_RESET_CLR(HW_RESETS_RESET_OFFSET_PADS_BANK0);

  while (!HW_RESETS_RESET_DONE_OK(HW_RESETS_RESET_OFFSET_IO_BANK0))
    ;
  while (!HW_RESETS_RESET_DONE_OK(HW_RESETS_RESET_OFFSET_PADS_BANK0))
    ;

  HW_GPIO_CTRL(HW_LED_GPIO) = 5;
  HW_SIO_GPIO_OE_SET(HW_LED_GPIO);
  HW_SIO_GPIO_OUT_SET(HW_LED_GPIO);

  s_usb_init_blocking();
  const char *data = "Hello world!";
  while (1) {
    for (volatile int i = 0; i < 1e6; i++)
      ;
    s_usb_cdc_send((const uint8_t *)data, strlen(data));
  }

  return 0;
}
