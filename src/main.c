typedef unsigned int uint32_t;

#define _IO_BANK0_BASE(offset) (0x40014000 + (offset))

#define GPIO_STAT(gpio) *(volatile uint32_t *)_IO_BANK0_BASE((gpio) * 8)
#define GPIO_CTRL(gpio) *(volatile uint32_t *)_IO_BANK0_BASE(((gpio) * 8) + 4)

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

#define _RESETS_BASE(offset) (0x4000c000 + (offset))

#define RESETS_RESET_RST 0x1
#define RESETS_WDSEL_RST 0x0
#define RESETS_DONE_RST 0x0

#define RESETS_RESET(peripheral) *(volatile uint32_t *)_RESETS_BASE(0x0)

#define RESETS_RESET_CLR(peripheral)                                           \
  RESETS_RESET(peripheral) &= ~(RESETS_RESET_RST << (peripheral))

#define RESETS_WDSEL(watchdog) *(volatile uint32_t *)_RESETS_BASE(0x4)

#define RESETS_WDSEL_CLR(watchdog)                                             \
  RESETS_WDSEL(watchdog) &= ~(RESETS_WDSEL_RST << (watchdog))

#define RESETS_RESET_DONE(done) *(volatile uint32_t *)_RESETS_BASE(0x8)

#define RESETS_RESET_DONE_OK(done) (RESETS_RESET_DONE(done) & (1 << (done)))

#define RESET_IO_BANK0 5
#define RESET_PADS_BANK0 8

#define _SIO_BASE(offset) (0xd0000000 + (offset))

#define SIO_GPIO_OUT_SET(gpio)                                                 \
  *(volatile uint32_t *)_SIO_BASE(0x14) = (1 << (gpio))

#define SIO_GPIO_OUT_CLR(gpio)                                                 \
  *(volatile uint32_t *)_SIO_BASE(0x18) = (1 << (gpio))

#define SIO_GPIO_OE_SET(gpio)                                                  \
  *(volatile uint32_t *)_SIO_BASE(0x024) = (1 << (gpio))

#define LED_GPIO 25

volatile uint32_t i = 0;

int main(void) {
  RESETS_RESET_CLR(RESET_IO_BANK0);
  RESETS_RESET_CLR(RESET_PADS_BANK0);

  while (!(RESETS_RESET_DONE_OK(RESET_IO_BANK0)))
    ;
  while (!(RESETS_RESET_DONE_OK(RESET_PADS_BANK0)))
    ;

  GPIO_CTRL(LED_GPIO) = 5;

  SIO_GPIO_OE_SET(LED_GPIO);
  SIO_GPIO_OUT_SET(LED_GPIO);

  while (1) {
    SIO_GPIO_OUT_SET(LED_GPIO);
    for (volatile int i = 0; i < 10000; i++)
      ;
    SIO_GPIO_OUT_CLR(LED_GPIO);
    for (volatile int i = 0; i < 10000; i++)
      ;
    i++;
  }
  return 0;
}
