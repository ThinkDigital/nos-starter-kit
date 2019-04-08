# kit-NB-IoT
  code example to give the first steps in NB IoT world using NB IoT KIT

## Processor
  STM32L073RZ (STElectronics)

## Modem
  BC68 (Quectel)

## Datasheet
  TODO: referenciar nome do documento...

## Example
  If you are already seeing this doc, you must to know that although, some classes are finished and have been tested in other projects they aren't yet compatible in this project and the main.cpp isn't finished.

  In next few days I will compile these files and set up an example to allow the kit to connect to network.

  If you want to start to work on code before that, you can use these files as guidelines.

  !! Notice that this code was built using SystemWorkbench from AC6 and probably it wouldn't be compatible with other IDEs. Which means that HAL functions have to be changed. !!

## 1st step
  Use STM32CubeMX to generate your HAL libraries according to the available board peripherals
  Configure USART clocks and gpios modes

## 2nd step
  Export your project to your preferred IDE
  In order to use the libraries in this project, you have to convert your project to cpp and rename your main.c to main.cpp

## 3rd step
  Copy only the files into the project that doesn't exist there our sync with git. If you're syncing with git, create your own .gitignore file
