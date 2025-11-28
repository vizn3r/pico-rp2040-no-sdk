extern unsigned int __data_start, __data_end, __data_load;
extern unsigned int __bss_start, __bss_end;
extern unsigned int _stack_top;

extern int main(void);

//__attribute__((noreturn, section(".text._start"))) void _start(void) {
//  unsigned int *src = &__data_load;
//  unsigned int *dst = &__data_start;
//  while (dst < &__data_end) {
//    *dst++ = *src++;
//  }
//
//  dst = &__bss_start;
//  while (dst < &__bss_end) {
//    *dst++ = 0;
//  }
//
//  main();
//
//  while (1)
//    ;
//}

__attribute__((naked, noreturn, section(".text._start"))) void _start(void) {
  __asm volatile("ldr r0, =_stack_top\n"
                 "mov sp, r0\n"
                 "b _c_startup\n");
}

__attribute__(()) void _c_startup(void) {
  unsigned int *src = &__data_load;
  unsigned int *dst = &__data_start;
  while (dst < &__data_end) {
    *dst++ = *src++;
  }

  dst = &__bss_start;
  while (dst < &__bss_end) {
    *dst++ = 0;
  }

  main();

  while (1)
    ;
}
