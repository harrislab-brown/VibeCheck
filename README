# VibeCheck Hardware Repository

VibeCheck is a vibration measurement system developed for the Vibration of Mechanical Systems at Brown.
While many of the features of VibeCheck are tailored specifically to meet the needs of the course laboratory component, the hardware
offers a robust solution for interrogating a wide range of vibrating mechanical systems. 

# Communication Interface
All communication between a host computer application and the MCU uses the onboard USB-C port and standard serial protocol. There are two main ways to communicate with the board. 

- Graphical web app:
[VibeCheck_Web](https://eli-silver.github.io/vibecheck_web/) is a companion web application for communicating with the VibeCheck hardware. It provides a simple interface to access all of the commonly used functions of the device. This includes plotting real-time acceleration data measured by the sensors and logging incoming data to a file in CSV format. 

- Serial Protocol:
Users can also communicate with the VibeCheck hardware directly using the serial interface. A complete list of the available commands and examples of their use can be found in the /Firmware/README file. 

# Hardware
The hardware was designed using [KiCad](https://www.kicad.org/), an open source electronics design tool. All design files can be found in the /VibeCheck/Hardware folder. 

There are two boards in this project, the mainboard that acts as a data acquisition and communication hub and sensor boards that can be used to measure acceleration and angular rate. Each mainboard can connect to three sensor boards. 

## Main board
- Microcontroller:
This project uses an STM32H723 microcontroller. This high performance MCU uses an Arm-Cortex M7 processor clocked at frequencies 
up to 550 MHz. It has 1MB of embedded flash memory and 564kB of SRAM. 

- USB:
The microcontroller has an integrated USB-FS physical layer. Communication to the hardware from a host computer uses the USB interface.

- Sensor communication:
Three SPI peripherals from the MCU are broken out to connectors. Thin cables carry SPI data between the mainboard and the sensor baord. 

- Waveform generator:
The MCU's stereo digital to analog output is used as a waveform generator. To buffer and boost the signal coming from the ADC pins, the board uses a headphone amplifier chip to drive a 3.5mm stereo headphone jack. A mute button on the board enables / disables the headphone amplifier chip.

- Strobe LEDs:
The board has 12 high-power white LEDs to act as a strobe. The strobe LEDs are driven using hardware timers on the MCU with logic-level FETs to switch the LED current. 

- Status LEDs:
The board has three addressible RGB LEDs per sensor channel. These LEDs are used to display status updates and visually indicate the current acceleration measured by the sensor boards. 

- GPIO:
A collection of MCU pins have been broken out to a GPIO header. This header includes power and ground pins, unused timer channels, and two digital to analog (DAC) pins.

- I2C:
A 5V I2C interface is broken out to a connector on the board. This connector follows the specifications used by the Adafruit STEMMA QT and SparkFun Qwiic line of boards. 

- Debug:
Programming and debugging of the board is accomplished using an ST-Link V3 Minie. a [Tag-Connect tc2030-nl](https://www.tag-connect.com/product/tc2030-ctx-nl-stdc14-for-use-with-stm32-processors-with-stlink-v3) footprint and associated debugger cable is used to connect the ST-Link to the board. 

## Sensor Board
The sensor board has an LSM6DS3 accelerometer. This MEMS device designed for mobile phone applications has a 3-axis accelerometer with selectable measurement ranges from +-2g to +- 16g and variable sample rates up to 6.6kHz. The sensor also has an onboard 3-axis angular rate sensor. 


# Firmware
Microcontroller firmware for this project is written in C. The STM32CubeIde development environment is used to write, flash, and debug code for this project. 

