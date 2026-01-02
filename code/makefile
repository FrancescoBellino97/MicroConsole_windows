#===============================================================================
# @file    makefile
# @author  Bellino Francesco
# @brief   Makefile for MicroConsole (Windows) project 
#
# BUILDING TARGET:
#	1) all: 			build the output file depending on the build configuration
#	2) cleanall:		remove the BUILD_DIR if present
#
# DESCRIPTION:
#	This makefile perform the following operations:
#	- create a BUILD_DIR directory
#	- compile all sources in APP_DIR into BUILD_SUPP_DIR
#	- link all the objects files into an .exe into BUILD_DIR
#	- generate the assembly code .lst into BUILD_DIR
#	- generate the memory allocation file .hdr into BUILD_DIR
#	- estimate the memory consumption with the tool MEM_USAGE
#	- print and save the memory consumtpio into BUILD_DIR
#	- copy the .DLL libraries into BUILD_DIR
#	
#===============================================================================


#===============================================================================#
#								BUILD OPTIONS									#
#===============================================================================#
# Project name
PROJECT_NAME		:=	MicroConsole

# Application and driver directories
APP_DIR				:=	core
LIB_DIR				:=	lib

# Build directory (generic and specific for target)
BUILD_DIR			:=	build
BUILD_SUPP_DIR		:=	$(BUILD_DIR)/support


# Object files list
OBJS 				:= 	$(BUILD_SUPP_DIR)/main.o \
						$(BUILD_SUPP_DIR)/emu.o \
						$(BUILD_SUPP_DIR)/cart.o \
						$(BUILD_SUPP_DIR)/bus.o


# Include directories list
INCS				:=	-I$(APP_DIR)/inc \
					-I$(LIB_DIR)/sdl2/include/SDL2 \
					-I$(LIB_DIR)/sdl2_ttf/include/SDL2

# Dependency files
DEPS				:=	$(OBJS:.o=.d)
-include $(DEPS)

# SDL library path
SDL_PATH        :=    $(LIB_DIR)\sdl2\lib\x64
SDL_TTF_PATH    :=    $(LIB_DIR)\sdl2_ttf\lib\x64

# Path for DLL copy at end of build process
DLLS			:=	$(BUILD_DIR)\SDL2.dll \
					$(BUILD_DIR)\SDL2_ttf.dll \
					$(BUILD_DIR)\libfreetype-6.dll


#===============================================================================#
#								POST-BUILD OPTIONS								#
#===============================================================================#
CC				:=	C:\msys64\ucrt64\bin\gcc
DLLFLAGS		:=	-lSDL2 -lSDL2_ttf -lfreetype-6 -L$(BUILD_DIR)
CFLAGS			:=	-Wall -Wextra -g
CPPFLAGS		:=	$(INCS)
DEPFLAGS		:=	-MMD -MP


#===============================================================================#
#								POST-BUILD OPTIONS								#
#===============================================================================#
MEM_USAGE		:=	tools\memoryUsage.exe
OBJDUMP			:=	C:\msys64\ucrt64\bin\objdump


###		POST-BUILD OPTIONS	###

MEM_USAGE		:=	tools\memoryUsage.exe
#===============================================================================#
#								BUILD REQUIREMENTS								#
#===============================================================================#
REQS			:=	$(BUILD_SUPP_DIR) \
					$(DLLS) \
					$(BUILD_DIR)/$(PROJECT_NAME).exe \
					$(BUILD_DIR)/$(PROJECT_NAME).lst \
					$(BUILD_DIR)/$(PROJECT_NAME).hdr \
					$(BUILD_DIR)/memory_usage.txt
					

#===============================================================================#
#								TARGETS											#
#===============================================================================#
.PHONY: all cleanall

all:	$(REQS)

cleanall:	#Delete directory recursively (/S) and without confirm (/Q)
	@if exist "$(BUILD_DIR)" rmdir /Q /S "$(BUILD_DIR)"
	@echo Deleting build directory


#===============================================================================#
#								BUILD PROCESS									#
#===============================================================================#
# EXE file build
$(BUILD_DIR)/$(PROJECT_NAME).exe: $(OBJS)
	@$(CC) $(CFLAGS) $(CPPFLAGS) $^ $(DLLFLAGS) -o $@ -Wl,-Map="$(BUILD_DIR)/$(PROJECT_NAME).map"
	@echo Creating $@

# Assembly code file generation
$(BUILD_DIR)/$(PROJECT_NAME).lst: $(BUILD_DIR)/$(PROJECT_NAME).exe
	@$(OBJDUMP) -S $^ > $@

# Memory allocation file generation
$(BUILD_DIR)/$(PROJECT_NAME).hdr: $(BUILD_DIR)/$(PROJECT_NAME).exe
	@$(OBJDUMP) -h $^ > $@

# Memory usage file generation
$(BUILD_DIR)/memory_usage.txt: $(BUILD_DIR)/$(PROJECT_NAME).hdr
	@$(MEM_USAGE) $^ > $@
	@$(MEM_USAGE) $^

# Source files build
$(BUILD_SUPP_DIR)/%.o: $(APP_DIR)/src/%.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) -c $< $(DLLFLAGS) -o $@
	@echo Compiling $<

# Build directory creation
$(BUILD_SUPP_DIR):
	@mkdir "$(BUILD_SUPP_DIR)\"
	@echo Creating build directory

# DLL transfer to build directory (only windows)
$(BUILD_DIR)\SDL2.dll:
	@copy $(SDL_PATH)\SDL2.dll $(BUILD_DIR) > NUL
	@echo Moving $@ to build directory

$(BUILD_DIR)\SDL2_ttf.dll:
	@copy $(SDL_TTF_PATH)\SDL2_ttf.dll $(BUILD_DIR) > NUL
	@echo Moving $@ to build directory

$(BUILD_DIR)\libfreetype-6.dll:
	@copy $(SDL_TTF_PATH)\libfreetype-6.dll $(BUILD_DIR) > NUL
	@echo Moving $@ to build directory