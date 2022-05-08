# ESP32-S3-EYE

ESP32-S3-EYE is an image and audio processing development platform based on Espressif chips. It contains development examples that may be applied in practical applications.

## Overview

ESP32-S3-EYE provides examples such as Human Face Detection, Human Face Recognition, Cat Face Detection, Wake Word Detection, Speech Commands Recognition, etc. You can develop a variety of practical applications based on these examples.

### Hardware

![A02-en](https://user-images.githubusercontent.com/10337553/155143732-deeb9ef0-8227-4b59-8a4a-90dac969e62f.png)


![A07](https://user-images.githubusercontent.com/10337553/167303523-9fa0c80a-f047-408c-8b5f-ac5a52b9cdc2.png)

### Software

#### Get ESP-IDF

ESP32-S3-EYE runs on ESP-IDF. For details on getting ESP-IDF, please refer to [ESP-IDF Programming Guide](https://idf.espressif.com/).

> Please use the latest version of ESP-IDF on the master branch.

#### Get ESP32-S3-EYE

Run the following commands in your terminal to download ESP32-S3-EYE:

```bash
git clone --recursive https://github.com/W00ng/ESP32-S3-EYE.git
```

> Remember to use ``git submodule update --recursive --init`` to pull and update submodules of ESP32-S3-EYE.

## Run Examples

All examples of ESP32-S3-EYE are stored in [examples](./examples) folder. 

To run the examples, you only need to perform [Step 1: Set the target chip] (#Step-1 Set the target chip) and [Step 2: Launch and monitor] (#Step-2 Launch and monitor).

### Step 1: Set the target chip

Open the terminal and go to any folder that stores examples (e.g. examples/human_face_detection/lcd). Run the following command to set the target chip: 

```bash
idf.py set-target esp32s3
```

### Step 2: Launch and monitor

Flash the program and launch IDF Monitor:

```bash
idf.py flash monitor
```

## Known Issues

ESP32-S3-EYE does not have a USB-to-UART bridge, which may cause a problem: If the program flashed onto the board keeps the chip rebooting, you cannot flash more programs onto the board.

In case this issue occurs, you need to:

Hold down BOOT button and press RST button, then release RST button first, and BOOT button next. In this way, the board enters Firmware Download mode and you can start flashing program onto the board.
After flashing the program, press RST button to start the program.

## Support

if you need any help, please connect: aemails@163.com
