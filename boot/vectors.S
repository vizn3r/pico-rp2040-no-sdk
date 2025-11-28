.syntax unified
.cpu cortex-m0plus
.thumb

.section .vectors, "ax"

.global vector_table
vector_table:
  .word _stack_top
  .word _start
  .word hang
  .word hang


.thumb_func
hang:
  b hang
