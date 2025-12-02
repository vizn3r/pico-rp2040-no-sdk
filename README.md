# RPI PI Pico Bare bones

*...without Pico SDK*
<br>
***...without C stdlib***

### Why

- Because my 'tistic brain wanted to do so. No SDKs, just RP2040 ~*and W25Q16JV*~ docs
- Because it's fun
- Because I wanna write a system, not a boring ~*REST API*~ LED blink on ATMega328p

### Current features:

- ~*Not*~ Working QSPI Flash for XIP :D
- LED blink
- ...

### Current goals (might change over time):

- ✅ Custom stage 2 bootloader
- Basic scheduler
- Serial interface
- Some form of a executable format
- Simple kernel for syscalls
- Memory manager
- Semi-custom stdlib (malloc, free, printf, scanf...)
