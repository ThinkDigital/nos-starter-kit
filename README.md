# NOS Starter Kit

Code sample to give the first steps in NB IoT world using NOS NB IoT KIT.

## Hardware

- Processor: **STElectronics STM32L073RZ**
- Modem: **Quectel BC68**

## Guide

https://www.thinkdigital.pt/downloads/NOS_Starter_Kit_CE.pdf

## Example

The [`main.cpp`](Core/Src/main.cpp) file shows an example of how to connect to NB-IoT network.

**Note**: _this code was built using SystemWorkbench from AC6 and probably wouldn't be compatible with other IDEs, meaning that HAL functions have to be changed_

### Step 1

- Use **STM32CubeMX** to generate your HAL libraries according to the available board peripherals;
- Check [images](doc/images) to check **uController** pinout configuration;
- Check [hardware](doc/hardware.md) to compare to your configuration;
- Configure USART clocks and gpios modes.

**Note**: _don't forget to enable interrupts during USART configurations - the example uses interrupts for USART communication_

### Step 2

- Export your project to your preferred IDE;
- In order to use the libraries in this project, you have to convert your project to cpp and rename your main.c to main.cpp.

### Step 3

- Copy only the files into the project that don't exist there or sync with git. If you're syncing with git, create your own `.gitignore` file;
- Compile your code and flash into your starter kit;
- Have fun :smile:
