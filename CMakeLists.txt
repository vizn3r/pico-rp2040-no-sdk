cmake_minimum_required(VERSION 3.10)

project(pico-vina C ASM)

set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_SYTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_SIZE arm-none-eabi-size)

set(CPU_FLAGS "-mcpu=cortex-m0plus -mthumb")

set(CMAKE_C_FLAGS "${CPU_FLAGS} -Wall -Wextra -O2 -fno-builtin")
set(CMAKE_ASM_FLAGS "${CPU_FLAGS}")

set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/linker.ld")
set(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT} -nostdlib -Wl,-Map=output.map")

include_directories(src)
add_subdirectory(src/kernel)
add_subdirectory(src/serial)
add_subdirectory(src/hal)

add_executable(firmware.elf
  boot/boot2_checked.S
  boot/vectors.S
  src/startup.c
)

target_link_libraries(firmware.elf kernel serial hal)

add_custom_command(TARGET firmware.elf POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O binary firmware.elf firmware.bin
  COMMAND ${CMAKE_SIZE} firmware.elf
  COMMENT "Building fimrware.bin"
)

add_custom_command(TARGET firmware.elf POST_BUILD
  COMMAND picotool uf2 convert firmware.elf firmware.uf2 --family rp2040
  COMMENT "Converting to .uf2"
)
