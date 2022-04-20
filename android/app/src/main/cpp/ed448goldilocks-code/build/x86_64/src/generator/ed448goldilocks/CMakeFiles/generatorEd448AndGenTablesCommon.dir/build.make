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
CMAKE_BINARY_DIR = /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86_64

# Utility rule file for generatorEd448AndGenTablesCommon.

# Include any custom commands dependencies for this target.
include src/generator/ed448goldilocks/CMakeFiles/generatorEd448AndGenTablesCommon.dir/compiler_depend.make

# Include the progress variables for this target.
include src/generator/ed448goldilocks/CMakeFiles/generatorEd448AndGenTablesCommon.dir/progress.make

src/generator/ed448goldilocks/CMakeFiles/generatorEd448AndGenTablesCommon: src/GENERATED/c/ed448goldilocks/scalar.c
src/generator/ed448goldilocks/CMakeFiles/generatorEd448AndGenTablesCommon: src/GENERATED/c/ed448goldilocks/decaf.c

src/GENERATED/c/ed448goldilocks/decaf.c: ../../src/generator/template.py
src/GENERATED/c/ed448goldilocks/decaf.c: ../../src/per_curve/decaf.tmpl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86_64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating code for ed448goldilocks/decaf.c"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86_64/src/generator/ed448goldilocks && /usr/bin/python2.7 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/template.py --per=curve --item=ed448goldilocks --guard=ed448goldilocks/decaf.c -o /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86_64/src/GENERATED/c/ed448goldilocks/decaf.c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/per_curve/decaf.tmpl.c

src/GENERATED/c/ed448goldilocks/scalar.c: ../../src/generator/template.py
src/GENERATED/c/ed448goldilocks/scalar.c: ../../src/per_curve/scalar.tmpl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86_64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating code for ed448goldilocks/scalar.c"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86_64/src/generator/ed448goldilocks && /usr/bin/python2.7 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/template.py --per=curve --item=ed448goldilocks --guard=ed448goldilocks/scalar.c -o /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86_64/src/GENERATED/c/ed448goldilocks/scalar.c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/per_curve/scalar.tmpl.c

generatorEd448AndGenTablesCommon: src/GENERATED/c/ed448goldilocks/decaf.c
generatorEd448AndGenTablesCommon: src/GENERATED/c/ed448goldilocks/scalar.c
generatorEd448AndGenTablesCommon: src/generator/ed448goldilocks/CMakeFiles/generatorEd448AndGenTablesCommon
generatorEd448AndGenTablesCommon: src/generator/ed448goldilocks/CMakeFiles/generatorEd448AndGenTablesCommon.dir/build.make
.PHONY : generatorEd448AndGenTablesCommon

# Rule to build all files generated by this target.
src/generator/ed448goldilocks/CMakeFiles/generatorEd448AndGenTablesCommon.dir/build: generatorEd448AndGenTablesCommon
.PHONY : src/generator/ed448goldilocks/CMakeFiles/generatorEd448AndGenTablesCommon.dir/build

src/generator/ed448goldilocks/CMakeFiles/generatorEd448AndGenTablesCommon.dir/clean:
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86_64/src/generator/ed448goldilocks && $(CMAKE_COMMAND) -P CMakeFiles/generatorEd448AndGenTablesCommon.dir/cmake_clean.cmake
.PHONY : src/generator/ed448goldilocks/CMakeFiles/generatorEd448AndGenTablesCommon.dir/clean

src/generator/ed448goldilocks/CMakeFiles/generatorEd448AndGenTablesCommon.dir/depend:
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86_64 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/ed448goldilocks /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86_64 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86_64/src/generator/ed448goldilocks /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86_64/src/generator/ed448goldilocks/CMakeFiles/generatorEd448AndGenTablesCommon.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/generator/ed448goldilocks/CMakeFiles/generatorEd448AndGenTablesCommon.dir/depend

