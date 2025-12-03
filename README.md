# RPI PI Pico Bare bones

*...without Pico SDK*
<br>
***...without C stdlib***

### Why

- Because my 'tistic brain wanted to do so. No SDKs, just RP2040 docs
- Because it's fun
- Because I wanna write a system, not a boring LED blink on ATMega328p

### Current features:

- Working QSPI Flash for XIP :D
- LED blink
- ...

### Current goals (might change over time):

- ✅ Custom stage 2 bootloader
- Simple kernel for syscalls
- Serial interface
- Hardware abstractions
- Basic scheduler
- Memory manager
- Semi-custom stdlib (malloc, free, printf, scanf...)
- Some form of a executable format
