# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.20.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.20.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a

# Utility rule file for generatorP448.

# Include any custom commands dependencies for this target.
include src/generator/ed448goldilocks/CMakeFiles/generatorP448.dir/compiler_depend.make

# Include the progress variables for this target.
include src/generator/ed448goldilocks/CMakeFiles/generatorP448.dir/progress.make

src/generator/ed448goldilocks/CMakeFiles/generatorP448: src/GENERATED/c/p448/f_field.h
src/generator/ed448goldilocks/CMakeFiles/generatorP448: src/GENERATED/c/p448/f_generic.c

src/GENERATED/c/p448/f_field.h: ../../src/generator/template.py
src/GENERATED/c/p448/f_field.h: ../../src/per_field/f_field.tmpl.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating code for p448/f_field.h"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator/ed448goldilocks && /usr/bin/python2.7 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/template.py --per=field --guard=p448/f_field.h --item=p448 -o /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/GENERATED/c/p448/f_field.h /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/per_field/f_field.tmpl.h

src/GENERATED/c/p448/f_generic.c: ../../src/generator/template.py
src/GENERATED/c/p448/f_generic.c: ../../src/per_field/f_generic.tmpl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating code for p448/f_generic.c"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator/ed448goldilocks && /usr/bin/python2.7 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/template.py --per=field --guard=p448/f_generic.c --item=p448 -o /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/GENERATED/c/p448/f_generic.c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/per_field/f_generic.tmpl.c

generatorP448: src/GENERATED/c/p448/f_field.h
generatorP448: src/GENERATED/c/p448/f_generic.c
generatorP448: src/generator/ed448goldilocks/CMakeFiles/generatorP448
generatorP448: src/generator/ed448goldilocks/CMakeFiles/generatorP448.dir/build.make
.PHONY : generatorP448

# Rule to build all files generated by this target.
src/generator/ed448goldilocks/CMakeFiles/generatorP448.dir/build: generatorP448
.PHONY : src/generator/ed448goldilocks/CMakeFiles/generatorP448.dir/build

src/generator/ed448goldilocks/CMakeFiles/generatorP448.dir/clean:
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator/ed448goldilocks && $(CMAKE_COMMAND) -P CMakeFiles/generatorP448.dir/cmake_clean.cmake
.PHONY : src/generator/ed448goldilocks/CMakeFiles/generatorP448.dir/clean

src/generator/ed448goldilocks/CMakeFiles/generatorP448.dir/depend:
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/ed448goldilocks /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator/ed448goldilocks /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator/ed448goldilocks/CMakeFiles/generatorP448.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/generator/ed448goldilocks/CMakeFiles/generatorP448.dir/depend

