# ESP32-S3-RGB-Panel

ESP32-S3-RGB-Panel is an image and audio processing development platform based on Espressif chips. It contains development examples that may be applied in practical applications.

## Overview

MCU: ESP32-S3

Flash: 8MB, Psram: 8MB

Display Interface： RGB565

Screen: 4.0-inch display with 480x480 resolution, capacitive touch panel gt911

Audio Codec: ES8311

## Hardware

![A00_1_en](https://user-images.githubusercontent.com/10337553/167306077-89350092-68f7-49cc-92f7-0d604405ac6f.png)

![A03_en](https://user-images.githubusercontent.com/10337553/167306096-f9c11c90-ed91-4f0f-b2c4-19f42dc6e1b9.png)

## Software

All the examples are stored in .../examples folder. Please build it with **ESP-IDF 5.0**
![cf9bb940-f3d0-4c77-aea2-1a70621b6525](https://github.com/W00ng/ESP32-S3-RGB-Panel/assets/10337553/e7529af7-b5f8-465e-9cbd-4f91f170ed51)

### Step 1: Enter the examples folder
Open the terminal and go to any folder that stores examples (e.g. lvgl_demo):

```bash
cd ...\examples\lvgl_demo
```

### Step 2: Build the example

```bash
idf.py build
```

### Step 3: Flash and launch monitor
Flash the program and launch IDF Monitor:

```bash
idf.py flash monitor
```

## Known Issues

ESP32-S3-RGB-Panel does not have a USB-to-UART bridge, which may cause a problem: If the program flashed onto the board keeps the chip rebooting, you cannot flash more programs onto the board.

In case this issue occurs, you need to:

Hold down BOOT button and press RST button, then release RST button first, and BOOT button next. In this way, the board enters Firmware Download mode and you can start flashing program onto the board.
After flashing the program, press RST button to start the program.

## Support

if you need any help, please contact: aemails@163.com
