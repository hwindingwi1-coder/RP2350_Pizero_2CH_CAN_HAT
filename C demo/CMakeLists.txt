cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico2)
set(PICO_PLATFORM rp2350)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(pico_sdk_import.cmake)

project(C_demo C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(C_demo
    main.c
    mcp2515.c
)

pico_enable_stdio_usb(C_demo 1)
pico_enable_stdio_uart(C_demo 1)

target_link_libraries(C_demo 
    pico_stdlib 
    hardware_spi
    hardware_gpio
)

pico_add_extra_outputs(C_demo)
