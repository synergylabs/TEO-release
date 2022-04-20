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
CMAKE_BINARY_DIR = /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86

# Include any dependencies generated for this target.
include src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/compiler_depend.make

# Include the progress variables for this target.
include src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/progress.make

# Include the compile flags for this target's objects.
include src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/flags.make

src/GENERATED/c/curve25519/decaf_gen_tables.c: ../../src/generator/template.py
src/GENERATED/c/curve25519/decaf_gen_tables.c: ../../src/per_curve/decaf_gen_tables.tmpl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating code for curve25519/decaf_gen_tables.c"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/generator/curve25519 && /usr/bin/python2.7 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/template.py --per=curve --item=curve25519 --guard=curve25519/decaf_gen_tables.c -o /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/GENERATED/c/curve25519/decaf_gen_tables.c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/per_curve/decaf_gen_tables.tmpl.c

src/GENERATED/c/curve25519/decaf.c: ../../src/generator/template.py
src/GENERATED/c/curve25519/decaf.c: ../../src/per_curve/decaf.tmpl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating code for curve25519/decaf.c"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/generator/curve25519 && /usr/bin/python2.7 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/template.py --per=curve --item=curve25519 --guard=curve25519/decaf.c -o /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/GENERATED/c/curve25519/decaf.c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/per_curve/decaf.tmpl.c

src/GENERATED/c/curve25519/scalar.c: ../../src/generator/template.py
src/GENERATED/c/curve25519/scalar.c: ../../src/per_curve/scalar.tmpl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Generating code for curve25519/scalar.c"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/generator/curve25519 && /usr/bin/python2.7 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/template.py --per=curve --item=curve25519 --guard=curve25519/scalar.c -o /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/GENERATED/c/curve25519/scalar.c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/per_curve/scalar.tmpl.c

src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf_gen_tables.c.o: src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/flags.make
src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf_gen_tables.c.o: src/GENERATED/c/curve25519/decaf_gen_tables.c
src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf_gen_tables.c.o: src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf_gen_tables.c.o"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/generator/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=i686-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf_gen_tables.c.o -MF CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf_gen_tables.c.o.d -o CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf_gen_tables.c.o -c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/GENERATED/c/curve25519/decaf_gen_tables.c

src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf_gen_tables.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf_gen_tables.c.i"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/generator/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=i686-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/GENERATED/c/curve25519/decaf_gen_tables.c > CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf_gen_tables.c.i

src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf_gen_tables.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf_gen_tables.c.s"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/generator/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=i686-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/GENERATED/c/curve25519/decaf_gen_tables.c -o CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf_gen_tables.c.s

src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf.c.o: src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/flags.make
src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf.c.o: src/GENERATED/c/curve25519/decaf.c
src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf.c.o: src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf.c.o"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/generator/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=i686-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf.c.o -MF CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf.c.o.d -o CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf.c.o -c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/GENERATED/c/curve25519/decaf.c

src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf.c.i"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/generator/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=i686-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/GENERATED/c/curve25519/decaf.c > CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf.c.i

src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf.c.s"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/generator/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=i686-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/GENERATED/c/curve25519/decaf.c -o CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf.c.s

src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/scalar.c.o: src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/flags.make
src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/scalar.c.o: src/GENERATED/c/curve25519/scalar.c
src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/scalar.c.o: src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/scalar.c.o"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/generator/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=i686-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/scalar.c.o -MF CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/scalar.c.o.d -o CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/scalar.c.o -c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/GENERATED/c/curve25519/scalar.c

src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/scalar.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/scalar.c.i"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/generator/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=i686-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/GENERATED/c/curve25519/scalar.c > CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/scalar.c.i

src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/scalar.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/scalar.c.s"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/generator/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=i686-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/GENERATED/c/curve25519/scalar.c -o CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/scalar.c.s

src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/utils.c.o: src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/flags.make
src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/utils.c.o: ../../src/utils.c
src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/utils.c.o: src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/utils.c.o"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/generator/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=i686-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/utils.c.o -MF CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/utils.c.o.d -o CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/utils.c.o -c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/utils.c

src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/utils.c.i"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/generator/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=i686-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/utils.c > CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/utils.c.i

src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/utils.c.s"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/generator/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=i686-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/utils.c -o CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/utils.c.s

# Object files for target decaf_gen_tables_curve25519
decaf_gen_tables_curve25519_OBJECTS = \
"CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf_gen_tables.c.o" \
"CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf.c.o" \
"CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/scalar.c.o" \
"CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/utils.c.o"

# External object files for target decaf_gen_tables_curve25519
decaf_gen_tables_curve25519_EXTERNAL_OBJECTS = \
"/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/p25519/CMakeFiles/p25519.dir/arch_32/f_impl.c.o" \
"/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/p25519/CMakeFiles/p25519.dir/f_arithmetic.c.o" \
"/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/p25519/CMakeFiles/p25519.dir/__/GENERATED/c/p25519/f_generic.c.o"

src/generator/curve25519/decaf_gen_tables_curve25519: src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf_gen_tables.c.o
src/generator/curve25519/decaf_gen_tables_curve25519: src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/decaf.c.o
src/generator/curve25519/decaf_gen_tables_curve25519: src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/GENERATED/c/curve25519/scalar.c.o
src/generator/curve25519/decaf_gen_tables_curve25519: src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/__/__/utils.c.o
src/generator/curve25519/decaf_gen_tables_curve25519: src/p25519/CMakeFiles/p25519.dir/arch_32/f_impl.c.o
src/generator/curve25519/decaf_gen_tables_curve25519: src/p25519/CMakeFiles/p25519.dir/f_arithmetic.c.o
src/generator/curve25519/decaf_gen_tables_curve25519: src/p25519/CMakeFiles/p25519.dir/__/GENERATED/c/p25519/f_generic.c.o
src/generator/curve25519/decaf_gen_tables_curve25519: src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/build.make
src/generator/curve25519/decaf_gen_tables_curve25519: src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking C executable decaf_gen_tables_curve25519"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/generator/curve25519 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/decaf_gen_tables_curve25519.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/build: src/generator/curve25519/decaf_gen_tables_curve25519
.PHONY : src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/build

src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/clean:
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/generator/curve25519 && $(CMAKE_COMMAND) -P CMakeFiles/decaf_gen_tables_curve25519.dir/cmake_clean.cmake
.PHONY : src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/clean

src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/depend: src/GENERATED/c/curve25519/decaf.c
src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/depend: src/GENERATED/c/curve25519/decaf_gen_tables.c
src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/depend: src/GENERATED/c/curve25519/scalar.c
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/generator/curve25519 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/generator/curve25519 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/generator/curve25519/CMakeFiles/decaf_gen_tables_curve25519.dir/depend

