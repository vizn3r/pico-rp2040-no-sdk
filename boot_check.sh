#!/bin/bash

mkdir -p build
rm ./boot/boot2_checked.S

cd build

arm-none-eabi-gcc -c ../boot/boot2.S -o boot2_temp.o
arm-none-eabi-objcopy -O binary boot2_temp.o boot2_raw.bin

dd if=boot2_raw.bin bs=1 count=252 of=boot2_trimmed.bin 2>/dev/null
../checksum.py -s 0xffffffff boot2_trimmed.bin ../boot/boot2_checked.S
