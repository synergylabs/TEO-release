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

# Utility rule file for generatorEd448goldilocks.

# Include any custom commands dependencies for this target.
include src/generator/ed448goldilocks/CMakeFiles/generatorEd448goldilocks.dir/compiler_depend.make

# Include the progress variables for this target.
include src/generator/ed448goldilocks/CMakeFiles/generatorEd448goldilocks.dir/progress.make

src/generator/ed448goldilocks/CMakeFiles/generatorEd448goldilocks: src/GENERATED/c/ed448goldilocks/elligator.c
src/generator/ed448goldilocks/CMakeFiles/generatorEd448goldilocks: src/GENERATED/c/ed448goldilocks/eddsa.c
src/generator/ed448goldilocks/CMakeFiles/generatorEd448goldilocks: src/GENERATED/include/decaf/point_448.hxx
src/generator/ed448goldilocks/CMakeFiles/generatorEd448goldilocks: src/GENERATED/include/decaf/ed448.hxx

src/GENERATED/c/ed448goldilocks/eddsa.c: ../../src/generator/template.py
src/GENERATED/c/ed448goldilocks/eddsa.c: ../../src/per_curve/eddsa.tmpl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating code for ed448goldilocks/eddsa.c"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator/ed448goldilocks && /usr/bin/python2.7 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/template.py --per=curve --item=ed448goldilocks --guard=ed448goldilocks/eddsa.c -o /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/GENERATED/c/ed448goldilocks/eddsa.c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/per_curve/eddsa.tmpl.c

src/GENERATED/c/ed448goldilocks/elligator.c: ../../src/generator/template.py
src/GENERATED/c/ed448goldilocks/elligator.c: ../../src/per_curve/elligator.tmpl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating code for ed448goldilocks/elligator.c"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator/ed448goldilocks && /usr/bin/python2.7 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/template.py --per=curve --item=ed448goldilocks --guard=ed448goldilocks/elligator.c -o /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/GENERATED/c/ed448goldilocks/elligator.c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/per_curve/elligator.tmpl.c

src/GENERATED/include/decaf/ed448.hxx: ../../src/generator/template.py
src/GENERATED/include/decaf/ed448.hxx: ../../src/per_curve/eddsa.tmpl.hxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Generating code for include/decaf/ed448.hxx"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator/ed448goldilocks && /usr/bin/python2.7 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/template.py --per=curve --item=ed448goldilocks --guard=decaf/ed448.hxx -o /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/GENERATED/include/decaf/ed448.hxx /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/per_curve/eddsa.tmpl.hxx

src/GENERATED/include/decaf/point_448.hxx: ../../src/generator/template.py
src/GENERATED/include/decaf/point_448.hxx: ../../src/per_curve/point.tmpl.hxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Generating code for include/decaf/point_448.hxx"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator/ed448goldilocks && /usr/bin/python2.7 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/template.py --per=curve --item=ed448goldilocks --guard=decaf/point_448.hxx -o /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/GENERATED/include/decaf/point_448.hxx /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/per_curve/point.tmpl.hxx

generatorEd448goldilocks: src/GENERATED/c/ed448goldilocks/eddsa.c
generatorEd448goldilocks: src/GENERATED/c/ed448goldilocks/elligator.c
generatorEd448goldilocks: src/GENERATED/include/decaf/ed448.hxx
generatorEd448goldilocks: src/GENERATED/include/decaf/point_448.hxx
generatorEd448goldilocks: src/generator/ed448goldilocks/CMakeFiles/generatorEd448goldilocks
generatorEd448goldilocks: src/generator/ed448goldilocks/CMakeFiles/generatorEd448goldilocks.dir/build.make
.PHONY : generatorEd448goldilocks

# Rule to build all files generated by this target.
src/generator/ed448goldilocks/CMakeFiles/generatorEd448goldilocks.dir/build: generatorEd448goldilocks
.PHONY : src/generator/ed448goldilocks/CMakeFiles/generatorEd448goldilocks.dir/build

src/generator/ed448goldilocks/CMakeFiles/generatorEd448goldilocks.dir/clean:
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator/ed448goldilocks && $(CMAKE_COMMAND) -P CMakeFiles/generatorEd448goldilocks.dir/cmake_clean.cmake
.PHONY : src/generator/ed448goldilocks/CMakeFiles/generatorEd448goldilocks.dir/clean

src/generator/ed448goldilocks/CMakeFiles/generatorEd448goldilocks.dir/depend:
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/ed448goldilocks /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator/ed448goldilocks /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator/ed448goldilocks/CMakeFiles/generatorEd448goldilocks.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/generator/ed448goldilocks/CMakeFiles/generatorEd448goldilocks.dir/depend
