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

# Utility rule file for decaf_tables_curve25519.

# Include any custom commands dependencies for this target.
include src/generator/curve25519/CMakeFiles/decaf_tables_curve25519.dir/compiler_depend.make

# Include the progress variables for this target.
include src/generator/curve25519/CMakeFiles/decaf_tables_curve25519.dir/progress.make

src/generator/curve25519/CMakeFiles/decaf_tables_curve25519: src/generator/curve25519/decaf_gen_tables_curve25519
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating code for curve25519/decaf_tables.c"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator/curve25519 && decaf_gen_tables_curve25519 > /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/curve25519/decaf_tables.c

decaf_tables_curve25519: src/generator/curve25519/CMakeFiles/decaf_tables_curve25519
decaf_tables_curve25519: src/generator/curve25519/CMakeFiles/decaf_tables_curve25519.dir/build.make
.PHONY : decaf_tables_curve25519

# Rule to build all files generated by this target.
src/generator/curve25519/CMakeFiles/decaf_tables_curve25519.dir/build: decaf_tables_curve25519
.PHONY : src/generator/curve25519/CMakeFiles/decaf_tables_curve25519.dir/build

src/generator/curve25519/CMakeFiles/decaf_tables_curve25519.dir/clean:
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator/curve25519 && $(CMAKE_COMMAND) -P CMakeFiles/decaf_tables_curve25519.dir/cmake_clean.cmake
.PHONY : src/generator/curve25519/CMakeFiles/decaf_tables_curve25519.dir/clean

src/generator/curve25519/CMakeFiles/decaf_tables_curve25519.dir/depend:
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/curve25519 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator/curve25519 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator/curve25519/CMakeFiles/decaf_tables_curve25519.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/generator/curve25519/CMakeFiles/decaf_tables_curve25519.dir/depend

