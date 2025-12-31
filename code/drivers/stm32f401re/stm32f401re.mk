#===============================================================================#
# @file    makefile
# @author  Bellino Francesco
# @brief   Makefile variables for STM32F401RE microcontroller
#
#===============================================================================#

#===============================================================================#
#								TOOLCHAIN PATH									#
#===============================================================================#
# The toolchain used can be downloaded from https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads
PATH				:=..\..\..\Toolchains\arm-gnu-toolchain-13.2.Rel1-mingw-w64-i686-arm-none-eabi


#===============================================================================#
#								BUILD COMMANDS									#
#===============================================================================#
CC					:=	$(PATH)\bin\arm-none-eabi-gcc
OBJDUMP				:=	$(PATH)\bin\arm-none-eabi-objdump


#===============================================================================#
#								COMPILER OPTIONS								#
#===============================================================================#
CFLAGS				:=	-mcpu=cortex-m4 -mthumb -nostdlib -Wall -Wextra
CPPFLAGS			:=	$(INCS)
LDFLAGS				:=	-T linkers/$(TARGET).ld
DEPFLAGS			:=	-MMD -MP
