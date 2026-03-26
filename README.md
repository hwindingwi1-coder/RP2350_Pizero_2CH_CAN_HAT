# RP2350 PiZero 2-CH CAN HAT

Firmware examples (C / Pico SDK and MicroPython) for an RP2350-based "Pi Zero form-factor" board/HAT with **two MCP2515 CAN controllers** on a shared SPI bus.

This repository demonstrates a **bidirectional cross-channel test** between two CAN channels on a single RP2350 board:
- CAN0 (CH1) transmits -> CAN1 (CH2) receives
- CAN1 (CH2) transmits -> CAN0 (CH1) receives

## Features

- **Dual CAN Channels**: Support for two independent CAN 2.0B interfaces via MCP2515.
- **Shared SPI Bus**: Efficient use of RP2350 peripherals.
- **Dual Language Support**: Examples provided in both C (Pico SDK) and MicroPython.
- **RP2350 Optimized**: Specifically designed for the new Raspberry Pi RP2350 microcontroller.

## Images

![RP2350 PiZero board](assets/images/rp2350-pizero-1.jpg)
*Waveshare RP2350-PiZero board*

![2-CH CAN HAT](assets/images/2-ch-can-hat-1.jpg)
*2-CH CAN HAT expansion*

## Hardware Requirements

| Component | Specification | Purchase Link |
|-----------|---------------|---------------|
| MCU Board | Waveshare RP2350-PiZero or compatible | [Waveshare Store](https://www.waveshare.com/rp2350-pizero.htm) |
| CAN Expansion | Waveshare 2-CH CAN HAT | [Waveshare Store](https://www.waveshare.com/2-ch-can-hat.htm) |

### Pin Mapping

| Function | RP2350 GPIO | Description |
|----------|-------------|-------------|
| **SPI SCK** | `GP10` | Shared SPI1 Clock |
| **SPI MOSI**| `GP11` | Shared SPI1 Data Out |
| **SPI MISO**| `GP12` | Shared SPI1 Data In |
| **CAN0 CS** | `GP8`  | Channel 1 Chip Select |
| **CAN1 CS** | `GP7`  | Channel 2 Chip Select |
| **CAN0 INT**| `GP23` | Channel 1 Interrupt |
| **CAN1 INT**| `GP25` | Channel 2 Interrupt / Heartbeat LED |

## Repository Layout

- `C demo/` - Raspberry Pi Pico SDK example (`main.c`) + minimal MCP2515 driver (`mcp2515.c/.h`)
- `micropython/` - MicroPython example (`main.py`) + MCP2515 driver (`mcp2515.py`)

## Hardware Setup (Cross-Channel Test)

For the built-in cross-channel test, wire the two CAN channels together:
- Connect **CANH (CH1) <-> CANH (CH2)**
- Connect **CANL (CH1) <-> CANL (CH2)**

> **Note**: Ensure a 120Ω termination resistor is present on the bus if not already included on the hardware.

## Pico SDK (C) Demo

### Prerequisites
- Raspberry Pi Pico SDK v2.0.0+ (for RP2350 support)
- CMake + Ninja
- ARM GCC toolchain

### Build & Flash
```sh
cd "C demo"
cmake -S . -B build -G Ninja
cmake --build build
```
Copy `C demo/build/C_demo.uf2` to the RP2350 in BOOTSEL mode.

## MicroPython Demo

### Prerequisites
- MicroPython firmware for RP2350 installed on the board.

### Run
Copy these files to the device:
- `micropython/main.py`
- `micropython/mcp2515.py`

## Troubleshooting

- **Bitrate Mismatch**: Ensure both controllers use the same bitrate (default 125 kbps).
- **Crystal Frequency**: If your MCP2515 uses a 16MHz crystal instead of 8MHz, you **must** update the `CNF` register values in the source code.
- **SPI Speed**: If experiencing data corruption, try lowering the SPI baudrate in the initialization code.
- **Termination**: CAN bus requires 120Ω termination at both ends. For short "bench" tests, one resistor is often sufficient.

## License

This project is licensed under the [MIT License](LICENSE).

