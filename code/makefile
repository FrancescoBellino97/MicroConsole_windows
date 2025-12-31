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

# Build directory (generic and specific for target)
BUILD_DIR			:=	build
BUILD_SUPP_DIR		:=	$(BUILD_DIR)/support


# Object files list
APP_OBJS 			:= 	$(BUILD_SUPP_DIR)/main.o


# Include directories list
INCS				:=	-I$(APP_DIR)/inc

# Dependency files
DEPS				:=	$(OBJS:.o=.d)
-include $(DEPS)

#===============================================================================#
#								POST-BUILD OPTIONS								#
#===============================================================================#
MEM_USAGE		:=	tools\memoryUsage.exe


#===============================================================================#
#								BUILD REQUIREMENTS								#
#===============================================================================#
REQS			:=	$(BUILD_SUPP_DIR) \
					$(BUILD_TARGET_DIR)/$(PROJECT_NAME).exe \
					$(BUILD_TARGET_DIR)/$(PROJECT_NAME).lst \
					$(BUILD_TARGET_DIR)/$(PROJECT_NAME).hdr \
					$(BUILD_TARGET_DIR)/memory_usage.txt
					

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
$(BUILD_TARGET_DIR)/$(PROJECT_NAME).exe: $(OBJS)
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $@ -Wl,-Map="$(BUILD_TARGET_DIR)/$(PROJECT_NAME).map"
	@echo Creating $@

# Assembly code file generation
$(BUILD_TARGET_DIR)/$(PROJECT_NAME).lst: $(BUILD_TARGET_DIR)/$(PROJECT_NAME).elf
	@$(OBJDUMP) -S $^ > $@

# Memory allocation file generation
$(BUILD_TARGET_DIR)/$(PROJECT_NAME).hdr: $(BUILD_TARGET_DIR)/$(PROJECT_NAME).elf
	@$(OBJDUMP) -h $^ > $@

# Memory usage file generation
$(BUILD_TARGET_DIR)/memory_usage.txt: $(BUILD_TARGET_DIR)/$(PROJECT_NAME).hdr
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