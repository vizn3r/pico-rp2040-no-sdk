.syntax unified
.cpu cortex-m0plus
.thumb

.extern s_usb_irq_handle

.section .vectors, "ax"

.global vector_table
vector_table:
  .word _stack_top        // SP
  .word _start            // Reset
  .word hang              // NMI
  .word hang              // HardFault
  .word 0                 // Reserved
  .word 0                 // Reserved
  .word 0                 // Reserved
  .word 0                 // Reserved
  .word 0                 // Reserved
  .word 0                 // Reserved
  .word 0                 // Reserved
  .word hang              // SVCall
  .word 0                 // Reserved
  .word 0                 // Reserved
  .word hang              // PendSV
  .word hang              // SysTick
  .word hang              // IRQ0 TIMER_IRQ_0
  .word hang              // IRQ1 TIMER_IRQ_1
  .word hang              // IRQ2 TIMER_IRQ_2
  .word hang              // IRQ3 TIMER_IRQ_3
  .word hang              // IRQ4 PWM_IRQ_WRAP
  .word usb_irq_handle    // IRQ5 USBCTL_IRQ
  .word hang              // IRQ6 XIP_IRQ
  .word hang              // IRQ7 PIO0_IRQ_0
  .word hang              // IRQ8 PIO0_IRQ_1
  .word hang              // IRQ9 PIO1_IRQ_0
  .word hang              // IRQ10 PIO1_IRQ_1
  .word hang              // IRQ11 DMA_IRQ_0
  .word hang              // IRQ12 DMA_IRQ_1
  .word hang              // IRQ13 IO_IRQ_BANK0
  .word hang              // IRQ14 IO_IRQ_QSPI
  .word hang              // IRQ15 SIO_IRQ_PROC0
  .word hang              // IRQ16 SIO_IRQ_PROC1
  .word hang              // IRQ17 CLOCKS_IRQ
  .word hang              // IRQ18 SPI0_IRQ
  .word hang              // IRQ19 SPI1_IRQ
  .word hang              // IRQ20 UART0_IRQ
  .word hang              // IRQ21 UART1_IRQ
  .word hang              // IRQ22 ADC_IRQ_FIFO
  .word hang              // IRQ23 I2C0_IRQ
  .word hang              // IRQ24 I2C1_IRQ
  .word hang              // IRQ25 RTC_IRQ


.thumb_func
hang:
  b hang

