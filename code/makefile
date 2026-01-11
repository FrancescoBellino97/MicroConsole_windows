#===============================================================================#
# @file    makefile
# @author  Bellino Francesco
# @brief   Makefile for MicroConsole (Windows) project 
#
# BUILDING TARGET:
#	1) all: 			build the executable
#	2) clean:			remove the application build directory
#	3) cleanall:		remove all build directories (appication, time and 
#						unit tests
#	4) time_test:		build and run the time estimation tests
#	5) unit_test:		build and run the unit tests
#
# DESCRIPTION:
#	This makefile perform the following operations:
#	- set project name
#	- set code directories (sources and headers)
#	- set build directory (depending on build target)
#	- set object files list (common)
#	- set object files list (depeding on build target)
#	- generate and include dependencies
#	- set include directories (common)
#	- set include directories (depeding on build target)
#	- set DLL source paths
#	- set DLL destination paths (copied before build)
#	- set build options
#	- set post-build tool paths
#	- define build targets
#	- define build processes
#	- define build directory creation process
#	- define DLL copy to build directory processes
#	
#===============================================================================#


#===============================================================================#
#								PROJECT SETUP									#
#===============================================================================#
# Project name
PROJECT_NAME		:=	MicroConsole


# Project directories
#Application directory
APP_DIR				:=	core
#Library directory
LIB_DIR				:=	lib
#Tests directory
TEST_DIR			:=	test
#Unit tests sub-directory
UNIT_DIR			:=	$(TEST_DIR)/unit
#Time tests sub-directory
TIME_DIR			:=	$(TEST_DIR)/time
#Unity library sub-directory (for unit tests)
UNITY_DIR			:=	$(TEST_DIR)/unity


# Build directories
#Build directory for application
BUILD_APP_DIR			:=	build
#Build directory for unit tests
BUILD_UNIT_DIR			:= 	$(TEST_DIR)/build/unit
#Build directory for time tests
BUILD_TIME_DIR			:= 	$(TEST_DIR)/build/time

# Set build directory according to build target
ifeq ($(MAKECMDGOALS), all)
	BUILD_DIR			=	$(BUILD_APP_DIR)
	BUILD_SUPP_DIR		=	$(BUILD_DIR)/support
endif
ifeq ($(MAKECMDGOALS), unit_test)
	BUILD_DIR			=	$(BUILD_UNIT_DIR)
	BUILD_SUPP_DIR		=	$(BUILD_DIR)/support
endif
ifeq ($(MAKECMDGOALS), time_test)
	BUILD_DIR			=	$(BUILD_TIME_DIR)
	BUILD_SUPP_DIR		=	$(BUILD_DIR)/support
endif


# Object files list (common)
OBJS 				:= 	$(BUILD_SUPP_DIR)/emu.o \
						$(BUILD_SUPP_DIR)/cart.o \
						$(BUILD_SUPP_DIR)/cpu.o \
						$(BUILD_SUPP_DIR)/bus.o

# Object files list (for application)
APP_OBJS			:=	$(BUILD_SUPP_DIR)/main.o

# Object files list (for unit test)
UNIT_OBJS			:=	$(BUILD_SUPP_DIR)/unit_main.o \
						$(BUILD_SUPP_DIR)/unity.o \
						$(BUILD_SUPP_DIR)/cpu_unit.o

# Object files list (for time test)
TIME_OBJS			:=	$(BUILD_SUPP_DIR)/time_main.o \
						$(BUILD_SUPP_DIR)/time_util.o \
						$(BUILD_SUPP_DIR)/cart_time.o 

# Object files list (for dependency inclusion)
ALL_OBJS			:=	$(OBJS)

# Add to ALL_OBJS only the file objects used (depending on build target)
ifeq ($(MAKECMDGOALS), all)
	ALL_OBJS		+=	$(APP_OBJS)
endif
ifeq ($(MAKECMDGOALS), unit_test)
	ALL_OBJS		+=	$(UNIT_OBJS)
endif
ifeq ($(MAKECMDGOALS), time_test)
	ALL_OBJS		+=	$(TIME_OBJS)
endif


# Dependency files generation
DEPS				=	$(ALL_OBJS:.o=.d)
# Dependency files inclusion
-include $(DEPS)


# Header directories list
INCS				:=	-I$(APP_DIR)/inc

# Add to include files list only the directory used (depending on build target)
ifeq ($(MAKECMDGOALS), all)
	INCS			+=	-I$(LIB_DIR)/sdl2/include/SDL2 \
						-I$(LIB_DIR)/sdl2_ttf/include/SDL2
endif
ifeq ($(MAKECMDGOALS), unit_test)
	INCS			+=	-I$(UNIT_DIR)/inc \
						-I$(UNITY_DIR)
endif
ifeq ($(MAKECMDGOALS), time_test)
	INCS			+=	-I$(TIME_DIR)/inc
endif


# SDL library path
SDL_PATH        :=    $(LIB_DIR)\sdl2\lib\x64
SDL_TTF_PATH    :=    $(LIB_DIR)\sdl2_ttf\lib\x64


# Path for DLL copy at end of build process
DLLS			:=	$(BUILD_DIR)\SDL2.dll \
					$(BUILD_DIR)\SDL2_ttf.dll \
					$(BUILD_DIR)\libfreetype-6.dll


#===============================================================================#
#								BUILD OPTIONS									#
#===============================================================================#
#Compiler command path
CC			:=	C:\msys64\ucrt64\bin\gcc

#DLL flag for SDL library
DLLFLAGS	:=	-lSDL2
#DLL flag for SDL TTF library
DLLFLAGS	+=	-lSDL2_ttf
#DLL flag for SDL_TTF FreeType library
DLLFLAGS	+=	-lfreetype-6
#DLL flag to set the DLL directory
DLLFLAGS	+=	-L$(BUILD_DIR)

#Compiler flag to add all warnings
CFLAGS		:=	-Wall
#Compiler flag to add extra warnings
CFLAGS		+=	-Wextra
#Compiler flag to debug informations in .exe
CFLAGS		+=	-g

#Path to directory to include
CPPFLAGS	:=	$(INCS)

#Compiler option for ???
DEPFLAGS	:=	-MMD -MP

#Define to expose structures and functions (only for unit tests)
ifeq ($(MAKECMDGOALS),unit_test)
	CFLAGS += -DUNIT_TEST
endif


#===============================================================================#
#								POST-BUILD TOOLS								#
#===============================================================================#
#Memory usage estimation script path
MEM_USAGE	:=	tools\memoryUsage.exe
#OBJDUMP command path
OBJDUMP		:=	C:\msys64\ucrt64\bin\objdump


#===============================================================================#
#								BUILD REQUIREMENTS								#
#===============================================================================#
#Requirements for application
REQS_APP		:=	$(BUILD_SUPP_DIR) \
					$(DLLS) \
					$(BUILD_DIR)/$(PROJECT_NAME).exe \
					$(BUILD_DIR)/$(PROJECT_NAME).lst \
					$(BUILD_DIR)/$(PROJECT_NAME).hdr \
					$(BUILD_DIR)/memory_usage.txt

#Requirements for unit tests
REQS_UNIT		:=	$(BUILD_SUPP_DIR) \
					$(BUILD_DIR)/$(PROJECT_NAME)_unit_test.exe

#Requirements for time tests
REQS_TIME		:=	$(BUILD_SUPP_DIR) \
					$(BUILD_DIR)/$(PROJECT_NAME)_time.exe
					

#===============================================================================#
#								TARGETS											#
#===============================================================================#
.PHONY: all clean cleanall unit_test time_test

#Application target
all:		$(REQS_APP)

#Clean application build directory
clean:	#Delete application build directory recursively (/S) and without confirm (/Q)
	@if exist "$(BUILD_APP_DIR)" rmdir /Q /S "$(BUILD_APP_DIR)"
	@echo Deleting $(BUILD_APP_DIR) directory

#Clean all build directory
cleanall:	#Delete all build directories recursively (/S) and without confirm (/Q)
	@if exist "$(BUILD_APP_DIR)" rmdir /Q /S "$(BUILD_APP_DIR)"
	@echo Deleting app build directory
	@if exist "$(BUILD_UNIT_DIR)" rmdir /Q /S "$(BUILD_UNIT_DIR)"
	@echo Deleting unit test build directory
	@if exist "$(BUILD_TIME_DIR)" rmdir /Q /S "$(BUILD_TIME_DIR)"
	@echo Deleting time test build directory

#Unit tests target
unit_test:	$(REQS_UNIT)
	./$(BUILD_DIR)/$(PROJECT_NAME)_unit_test.exe

#Time estimation target
time_test:	$(REQS_TIME)
	./$(BUILD_DIR)/$(PROJECT_NAME)_time.exe


#===============================================================================#
#								BUILD PROCESS									#
#===============================================================================#
# EXE file build (application target)
$(BUILD_DIR)/$(PROJECT_NAME).exe: $(OBJS) $(APP_OBJS)
	@$(CC) $(CFLAGS) $(CPPFLAGS) $^ $(DLLFLAGS) -o $@ -Wl,-Map="$(BUILD_DIR)/$(PROJECT_NAME).map"
	@echo Creating $@

# EXE file build (unit test target)
$(BUILD_DIR)/$(PROJECT_NAME)_unit_test.exe: $(OBJS) $(UNIT_OBJS)
	@$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@
	@echo Creating $@

# EXE file build (time_test target)
$(BUILD_DIR)/$(PROJECT_NAME)_time.exe: $(OBJS) $(TIME_OBJS)
	@$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@
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

# Application source files build
$(BUILD_SUPP_DIR)/%.o: $(APP_DIR)/src/%.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) -c $< $(DLLFLAGS) -o $@
	@echo Compiling $<
	
# Unit test source files build
$(BUILD_SUPP_DIR)/%.o: $(UNIT_DIR)/src/%.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) -c $< $(DLLFLAGS) -o $@
	@echo Compiling $<
	
# Time test source files build
$(BUILD_SUPP_DIR)/%.o: $(TIME_DIR)/src/%.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) -c $< $(DLLFLAGS) -o $@
	@echo Compiling $<
	
# Unity source files build
$(BUILD_SUPP_DIR)/%.o: $(UNITY_DIR)/%.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) -c $< $(DLLFLAGS) -o $@
	@echo Compiling $<

# Build directory creation
$(BUILD_SUPP_DIR):
	@mkdir "$(BUILD_SUPP_DIR)\"
	@echo Creating build directory

# SDL2 transfer to build directory
$(BUILD_DIR)\SDL2.dll:
	@copy $(SDL_PATH)\SDL2.dll $(BUILD_DIR) > NUL
	@echo Moving $@ to build directory

# SDL2 TTF transfer to build directory
$(BUILD_DIR)\SDL2_ttf.dll:
	@copy $(SDL_TTF_PATH)\SDL2_ttf.dll $(BUILD_DIR) > NUL
	@echo Moving $@ to build directory

# SDL2 TTF FreeType transfer to build directory
$(BUILD_DIR)\libfreetype-6.dll:
	@copy $(SDL_TTF_PATH)\libfreetype-6.dll $(BUILD_DIR) > NUL
	@echo Moving $@ to build directory