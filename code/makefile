#===============================================================================
# @file    makefile
# @author  Bellino Francesco
# @brief   Makefile for MicroConsole (Windows) project 
#
# BUILDING TARGET:
#	1) all: 			build the output file depending on the build configuration
#	2) clean:			remove the build directory depending on the build configuration
#	3) cleanall:		remove the BUILD_DIR if present
#
# WORKFLOW: TODO: Update description
#  1) Define the project name, application and driver directories,
#	  build directory, 
#
#  1) Define the project name, application directory, library directory,
#     headers directory and build output directory.
#  2) Define the object file list and the directory of header files.
#  3) Define the dependency variable (for each object file a .d dependecy file 
#	  is generated (they are considered makefiles).
#  4) Read all dependencies with the "include" command (the - is used to 
#	  suppress errors that are generated when the dependency files are
#	  missing, the command is invoked multiple times).
#  5) Define the compiler variable, the compiler options variable and the 
#     dependency variable.
#  6) Create the build directory if not exist.
#  7) Define the "all" and "cleanall" targets.
#
#===============================================================================


#===============================================================================#
#								BUILD OPTIONS									#
#===============================================================================#
# Project name
PROJECT_NAME		:=	MicroConsole

# Application and driver directories
APP_DIR				:=	core
DRV_DIR				:=	drivers

# Build directory (generic and specific for target)
BUILD_DIR			:=	build
BUILD_SUPP_DIR		:=	$(BUILD_DIR)/support


# Object files list
APP_OBJS 			:= 	$(BUILD_SUPP_DIR)/main.o


# Include directories list
INCS				:=	-I$(APP_DIR)/inc \
						-I$(DRV_DIR)/inc

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
.PHONY: all cleanall clean

all:	$(REQS)

cleanall:	#Delete directory recursively (/S) and without confirm (/Q)
	@if exist "$(BUILD_DIR)" rmdir /Q /S "$(BUILD_DIR)"
	@echo Deleting build directory
	

clean:		#Delete target directory recursively (/S) and without confirm (/Q)
	@if exist "$(BUILD_TARGET_DIR)" rmdir /Q /S "$(BUILD_TARGET_DIR)"
	@echo Deleting target build directory


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

# Application files build
$(BUILD_SUPP_DIR)/%.o: $(APP_DIR)/src/%.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) -c $< $(DLLFLAGS) -o $@
	@echo Compiling $<

# Driver files build
$(BUILD_SUPP_DIR)/%.o: $(DRV_DIR)/src/%.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) -c $< $(DLLFLAGS) -o $@
	@echo Compiling $<

# Build directory creation
$(BUILD_SUPP_DIR):
	@mkdir "$(BUILD_SUPP_DIR)\"
	@echo Creating build directory