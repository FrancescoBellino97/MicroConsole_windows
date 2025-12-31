# MicroConsole project

## Scope
The goal of this project is to create an emulator of GameBoy that can run on multiple targets.

## Documentation
The reference documentation can be found at [PanDocs](https://gbdev.io/pandocs/).

## How it works
The project is structured to have a common part and a specific part. Creating a build configuration on Eclipse, the code can be built for different targets.

The project has the following structure:
 - **core**, here there is the common code
 - **drivers**, here there are sub-directories with the target specific code and makefiles.
 - **linkers**, here there are the linkerscripts for the microcontrollers
 - **tools**, here there is a memory estimation script
 - **makefile**, this is the main makefile

**Note**: The build configuration name is passed as TARGET parameter with make command and it is used to access the target specific directory name and files, so they must be the same name. Toolchain is not in the repository, but there are informations in the specific makefiles

## Target supported
This project support the following targets:
 - Windows 10 (ongoing)
 - STM32F401RE (to be evaluated)
 - ESP32 (to be evaluated)
 - ATmega328P (to be evaluated)
 
## Planning
This is the development plan:
 - Develope CPU emulation for Windows 10 (TBD)
 - Develope PPU emulation for Windows 10 (TBD)
 - Develope APU emulation for Windows 10 (TBD)
 - ...