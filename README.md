# kit-NB-IoT
  code example to give the first steps in NB IoT world using NB IoT KIT

## Processor
  STM32L073RZ (STElectronics)

## Modem
  BC68 (Quectel)

## Guideline
  https://www.thinkdigital.pt/downloads/NOS_Starter_Kit_CE.pdf

## Example
  The main.cpp file shows an example of how to connect to NB-IoT network.

  !! Notice that this code was built using SystemWorkbench from AC6 and probably it wouldn't be compatible with other IDEs. Which means that HAL functions have to be changed. !!

## 1st step
  Use STM32CubeMX to generate your HAL libraries according to the available board peripherals
  Check /doc/images/ to check uController pinout configuration
  Use /doc/hardware to compare to your configuration
  Configure USART clocks and gpios modes
  !! Don't forget to enable interrupts during usart configurations. The example uses interrupts for usart communication.

## 2nd step
  Export your project to your preferred IDE
  In order to use the libraries in this project, you have to convert your project to cpp and rename your main.c to main.cpp

## 3rd step
  Copy only the files into the project that doesn't exist there or sync with git. If you're syncing with git, create your own .gitignore file

## 4th
  Compile your code

## 5th
  Flash into your starter kit

## 6th
  Have fun :)
