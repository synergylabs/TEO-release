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

# Utility rule file for generatorCommonCode.

# Include any custom commands dependencies for this target.
include src/generator/CMakeFiles/generatorCommonCode.dir/compiler_depend.make

# Include the progress variables for this target.
include src/generator/CMakeFiles/generatorCommonCode.dir/progress.make

src/generator/CMakeFiles/generatorCommonCode: src/GENERATED/include/decaf.h
src/generator/CMakeFiles/generatorCommonCode: src/GENERATED/include/decaf/eddsa.hxx
src/generator/CMakeFiles/generatorCommonCode: src/GENERATED/include/decaf.hxx
src/generator/CMakeFiles/generatorCommonCode: src/GENERATED/c/decaf/crypto.hxx
src/generator/CMakeFiles/generatorCommonCode: src/GENERATED/include/decaf/common.h
src/generator/CMakeFiles/generatorCommonCode: src/GENERATED/include/decaf/sha512.hxx
src/generator/CMakeFiles/generatorCommonCode: src/GENERATED/include/decaf/sha512.h
src/generator/CMakeFiles/generatorCommonCode: src/GENERATED/include/decaf/shake.hxx
src/generator/CMakeFiles/generatorCommonCode: src/GENERATED/include/decaf/shake.h
src/generator/CMakeFiles/generatorCommonCode: src/GENERATED/include/decaf/spongerng.hxx
src/generator/CMakeFiles/generatorCommonCode: src/GENERATED/include/decaf/spongerng.h
src/generator/CMakeFiles/generatorCommonCode: src/GENERATED/include/decaf/secure_buffer.hxx
src/generator/CMakeFiles/generatorCommonCode: src/GENERATED/include/decaf/point_255.h
src/generator/CMakeFiles/generatorCommonCode: src/GENERATED/include/decaf/point_448.h
src/generator/CMakeFiles/generatorCommonCode: src/GENERATED/include/decaf/ed255.h
src/generator/CMakeFiles/generatorCommonCode: src/GENERATED/include/decaf/ed448.h

src/GENERATED/c/decaf/crypto.hxx: ../../src/generator/template.py
src/GENERATED/c/decaf/crypto.hxx: ../../src/include/decaf/crypto.tmpl.hxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating code for decaf/crypto.hxx"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator && /usr/bin/python2.7 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/template.py --per=global --guard=decaf/crypto.hxx -o /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/GENERATED/c/decaf/crypto.hxx /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/include/decaf/crypto.tmpl.hxx

src/GENERATED/include/decaf.h: ../../src/generator/template.py
src/GENERATED/include/decaf.h: ../../src/public_include/decaf.tmpl.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating code for include/decaf.h"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator && /usr/bin/python2.7 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/template.py --per=global --guard=decaf.h -o /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/GENERATED/include/decaf.h /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/public_include/decaf.tmpl.h

src/GENERATED/include/decaf.hxx: ../../src/generator/template.py
src/GENERATED/include/decaf.hxx: ../../src/public_include/decaf.tmpl.hxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Generating code for include/decaf.hxx"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator && /usr/bin/python2.7 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/template.py --per=global --guard=decaf.hxx -o /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/GENERATED/include/decaf.hxx /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/public_include/decaf.tmpl.hxx

src/GENERATED/include/decaf/common.h: ../../src/public_include/decaf/common.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Generating ../GENERATED/include/decaf/common.h"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator && /usr/local/Cellar/cmake/3.20.2/bin/cmake -E copy /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/public_include/decaf/common.h /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/GENERATED/include/decaf/common.h

src/GENERATED/include/decaf/ed255.h: ../../src/generator/template.py
src/GENERATED/include/decaf/ed255.h: ../../src/per_curve/eddsa.tmpl.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Generating code for decaf/ed255.h"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator && /usr/bin/python2.7 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/template.py --per=curve --item=curve25519 --guard=decaf/ed255.h -o /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/GENERATED/include/decaf/ed255.h /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/per_curve/eddsa.tmpl.h

src/GENERATED/include/decaf/ed448.h: ../../src/generator/template.py
src/GENERATED/include/decaf/ed448.h: ../../src/per_curve/eddsa.tmpl.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Generating code for decaf/ed448.h"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator && /usr/bin/python2.7 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/template.py --per=curve --item=ed448goldilocks --guard=decaf/ed448.h -o /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/GENERATED/include/decaf/ed448.h /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/per_curve/eddsa.tmpl.h

src/GENERATED/include/decaf/eddsa.hxx: ../../src/generator/template.py
src/GENERATED/include/decaf/eddsa.hxx: ../../src/public_include/decaf/eddsa.tmpl.hxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Generating code for include/decaf/eddsa.hxx"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator && /usr/bin/python2.7 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/template.py --per=global --guard=decaf/eddsa.hxx -o /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/GENERATED/include/decaf/eddsa.hxx /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/public_include/decaf/eddsa.tmpl.hxx

src/GENERATED/include/decaf/point_255.h: ../../src/generator/template.py
src/GENERATED/include/decaf/point_255.h: ../../src/per_curve/point.tmpl.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Generating code for decaf/point_255.h"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator && /usr/bin/python2.7 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/template.py --per=curve --item=curve25519 --guard=decaf/point_255.h -o /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/GENERATED/include/decaf/point_255.h /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/per_curve/point.tmpl.h

src/GENERATED/include/decaf/point_448.h: ../../src/generator/template.py
src/GENERATED/include/decaf/point_448.h: ../../src/per_curve/point.tmpl.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Generating code for decaf/point_448.h"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator && /usr/bin/python2.7 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/template.py --per=curve --item=ed448goldilocks --guard=decaf/point_448.h -o /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/GENERATED/include/decaf/point_448.h /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/per_curve/point.tmpl.h

src/GENERATED/include/decaf/secure_buffer.hxx: ../../src/public_include/decaf/secure_buffer.hxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Generating ../GENERATED/include/decaf/secure_buffer.hxx"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator && /usr/local/Cellar/cmake/3.20.2/bin/cmake -E copy /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/public_include/decaf/secure_buffer.hxx /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/GENERATED/include/decaf/secure_buffer.hxx

src/GENERATED/include/decaf/sha512.h: ../../src/public_include/decaf/sha512.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Generating ../GENERATED/include/decaf/sha512.h"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator && /usr/local/Cellar/cmake/3.20.2/bin/cmake -E copy /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/public_include/decaf/sha512.h /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/GENERATED/include/decaf/sha512.h

src/GENERATED/include/decaf/sha512.hxx: ../../src/public_include/decaf/sha512.hxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Generating ../GENERATED/include/decaf/sha512.hxx"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator && /usr/local/Cellar/cmake/3.20.2/bin/cmake -E copy /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/public_include/decaf/sha512.hxx /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/GENERATED/include/decaf/sha512.hxx

src/GENERATED/include/decaf/shake.h: ../../src/public_include/decaf/shake.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Generating ../GENERATED/include/decaf/shake.h"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator && /usr/local/Cellar/cmake/3.20.2/bin/cmake -E copy /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/public_include/decaf/shake.h /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/GENERATED/include/decaf/shake.h

src/GENERATED/include/decaf/shake.hxx: ../../src/public_include/decaf/shake.hxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Generating ../GENERATED/include/decaf/shake.hxx"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator && /usr/local/Cellar/cmake/3.20.2/bin/cmake -E copy /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/public_include/decaf/shake.hxx /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/GENERATED/include/decaf/shake.hxx

src/GENERATED/include/decaf/spongerng.h: ../../src/public_include/decaf/spongerng.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Generating ../GENERATED/include/decaf/spongerng.h"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator && /usr/local/Cellar/cmake/3.20.2/bin/cmake -E copy /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/public_include/decaf/spongerng.h /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/GENERATED/include/decaf/spongerng.h

src/GENERATED/include/decaf/spongerng.hxx: ../../src/public_include/decaf/spongerng.hxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Generating ../GENERATED/include/decaf/spongerng.hxx"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator && /usr/local/Cellar/cmake/3.20.2/bin/cmake -E copy /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/public_include/decaf/spongerng.hxx /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/GENERATED/include/decaf/spongerng.hxx

generatorCommonCode: src/GENERATED/c/decaf/crypto.hxx
generatorCommonCode: src/GENERATED/include/decaf.h
generatorCommonCode: src/GENERATED/include/decaf.hxx
generatorCommonCode: src/GENERATED/include/decaf/common.h
generatorCommonCode: src/GENERATED/include/decaf/ed255.h
generatorCommonCode: src/GENERATED/include/decaf/ed448.h
generatorCommonCode: src/GENERATED/include/decaf/eddsa.hxx
generatorCommonCode: src/GENERATED/include/decaf/point_255.h
generatorCommonCode: src/GENERATED/include/decaf/point_448.h
generatorCommonCode: src/GENERATED/include/decaf/secure_buffer.hxx
generatorCommonCode: src/GENERATED/include/decaf/sha512.h
generatorCommonCode: src/GENERATED/include/decaf/sha512.hxx
generatorCommonCode: src/GENERATED/include/decaf/shake.h
generatorCommonCode: src/GENERATED/include/decaf/shake.hxx
generatorCommonCode: src/GENERATED/include/decaf/spongerng.h
generatorCommonCode: src/GENERATED/include/decaf/spongerng.hxx
generatorCommonCode: src/generator/CMakeFiles/generatorCommonCode
generatorCommonCode: src/generator/CMakeFiles/generatorCommonCode.dir/build.make
.PHONY : generatorCommonCode

# Rule to build all files generated by this target.
src/generator/CMakeFiles/generatorCommonCode.dir/build: generatorCommonCode
.PHONY : src/generator/CMakeFiles/generatorCommonCode.dir/build

src/generator/CMakeFiles/generatorCommonCode.dir/clean:
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator && $(CMAKE_COMMAND) -P CMakeFiles/generatorCommonCode.dir/cmake_clean.cmake
.PHONY : src/generator/CMakeFiles/generatorCommonCode.dir/clean

src/generator/CMakeFiles/generatorCommonCode.dir/depend:
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/armeabi-v7a/src/generator/CMakeFiles/generatorCommonCode.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/generator/CMakeFiles/generatorCommonCode.dir/depend
