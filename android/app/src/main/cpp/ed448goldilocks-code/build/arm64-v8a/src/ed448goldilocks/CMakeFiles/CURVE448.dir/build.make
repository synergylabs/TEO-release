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
CMAKE_BINARY_DIR = /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a

# Include any dependencies generated for this target.
include src/ed448goldilocks/CMakeFiles/CURVE448.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/ed448goldilocks/CMakeFiles/CURVE448.dir/compiler_depend.make

# Include the progress variables for this target.
include src/ed448goldilocks/CMakeFiles/CURVE448.dir/progress.make

# Include the compile flags for this target's objects.
include src/ed448goldilocks/CMakeFiles/CURVE448.dir/flags.make

src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/decaf.c.o: src/ed448goldilocks/CMakeFiles/CURVE448.dir/flags.make
src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/decaf.c.o: src/GENERATED/c/ed448goldilocks/decaf.c
src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/decaf.c.o: src/ed448goldilocks/CMakeFiles/CURVE448.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/decaf.c.o"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/ed448goldilocks && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/decaf.c.o -MF CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/decaf.c.o.d -o CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/decaf.c.o -c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/ed448goldilocks/decaf.c

src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/decaf.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/decaf.c.i"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/ed448goldilocks && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/ed448goldilocks/decaf.c > CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/decaf.c.i

src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/decaf.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/decaf.c.s"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/ed448goldilocks && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/ed448goldilocks/decaf.c -o CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/decaf.c.s

src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/elligator.c.o: src/ed448goldilocks/CMakeFiles/CURVE448.dir/flags.make
src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/elligator.c.o: src/GENERATED/c/ed448goldilocks/elligator.c
src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/elligator.c.o: src/ed448goldilocks/CMakeFiles/CURVE448.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/elligator.c.o"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/ed448goldilocks && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/elligator.c.o -MF CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/elligator.c.o.d -o CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/elligator.c.o -c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/ed448goldilocks/elligator.c

src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/elligator.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/elligator.c.i"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/ed448goldilocks && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/ed448goldilocks/elligator.c > CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/elligator.c.i

src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/elligator.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/elligator.c.s"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/ed448goldilocks && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/ed448goldilocks/elligator.c -o CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/elligator.c.s

src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/scalar.c.o: src/ed448goldilocks/CMakeFiles/CURVE448.dir/flags.make
src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/scalar.c.o: src/GENERATED/c/ed448goldilocks/scalar.c
src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/scalar.c.o: src/ed448goldilocks/CMakeFiles/CURVE448.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/scalar.c.o"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/ed448goldilocks && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/scalar.c.o -MF CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/scalar.c.o.d -o CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/scalar.c.o -c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/ed448goldilocks/scalar.c

src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/scalar.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/scalar.c.i"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/ed448goldilocks && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/ed448goldilocks/scalar.c > CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/scalar.c.i

src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/scalar.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/scalar.c.s"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/ed448goldilocks && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/ed448goldilocks/scalar.c -o CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/scalar.c.s

src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/eddsa.c.o: src/ed448goldilocks/CMakeFiles/CURVE448.dir/flags.make
src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/eddsa.c.o: src/GENERATED/c/ed448goldilocks/eddsa.c
src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/eddsa.c.o: src/ed448goldilocks/CMakeFiles/CURVE448.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/eddsa.c.o"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/ed448goldilocks && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/eddsa.c.o -MF CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/eddsa.c.o.d -o CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/eddsa.c.o -c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/ed448goldilocks/eddsa.c

src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/eddsa.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/eddsa.c.i"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/ed448goldilocks && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/ed448goldilocks/eddsa.c > CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/eddsa.c.i

src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/eddsa.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/eddsa.c.s"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/ed448goldilocks && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/ed448goldilocks/eddsa.c -o CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/eddsa.c.s

src/ed448goldilocks/CMakeFiles/CURVE448.dir/decaf_tables.c.o: src/ed448goldilocks/CMakeFiles/CURVE448.dir/flags.make
src/ed448goldilocks/CMakeFiles/CURVE448.dir/decaf_tables.c.o: ../../src/ed448goldilocks/decaf_tables.c
src/ed448goldilocks/CMakeFiles/CURVE448.dir/decaf_tables.c.o: src/ed448goldilocks/CMakeFiles/CURVE448.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object src/ed448goldilocks/CMakeFiles/CURVE448.dir/decaf_tables.c.o"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/ed448goldilocks && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/ed448goldilocks/CMakeFiles/CURVE448.dir/decaf_tables.c.o -MF CMakeFiles/CURVE448.dir/decaf_tables.c.o.d -o CMakeFiles/CURVE448.dir/decaf_tables.c.o -c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/ed448goldilocks/decaf_tables.c

src/ed448goldilocks/CMakeFiles/CURVE448.dir/decaf_tables.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/CURVE448.dir/decaf_tables.c.i"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/ed448goldilocks && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/ed448goldilocks/decaf_tables.c > CMakeFiles/CURVE448.dir/decaf_tables.c.i

src/ed448goldilocks/CMakeFiles/CURVE448.dir/decaf_tables.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/CURVE448.dir/decaf_tables.c.s"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/ed448goldilocks && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/ed448goldilocks/decaf_tables.c -o CMakeFiles/CURVE448.dir/decaf_tables.c.s

CURVE448: src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/decaf.c.o
CURVE448: src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/elligator.c.o
CURVE448: src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/scalar.c.o
CURVE448: src/ed448goldilocks/CMakeFiles/CURVE448.dir/__/GENERATED/c/ed448goldilocks/eddsa.c.o
CURVE448: src/ed448goldilocks/CMakeFiles/CURVE448.dir/decaf_tables.c.o
CURVE448: src/ed448goldilocks/CMakeFiles/CURVE448.dir/build.make
.PHONY : CURVE448

# Rule to build all files generated by this target.
src/ed448goldilocks/CMakeFiles/CURVE448.dir/build: CURVE448
.PHONY : src/ed448goldilocks/CMakeFiles/CURVE448.dir/build

src/ed448goldilocks/CMakeFiles/CURVE448.dir/clean:
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/ed448goldilocks && $(CMAKE_COMMAND) -P CMakeFiles/CURVE448.dir/cmake_clean.cmake
.PHONY : src/ed448goldilocks/CMakeFiles/CURVE448.dir/clean

src/ed448goldilocks/CMakeFiles/CURVE448.dir/depend:
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/ed448goldilocks /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/ed448goldilocks /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/ed448goldilocks/CMakeFiles/CURVE448.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/ed448goldilocks/CMakeFiles/CURVE448.dir/depend

