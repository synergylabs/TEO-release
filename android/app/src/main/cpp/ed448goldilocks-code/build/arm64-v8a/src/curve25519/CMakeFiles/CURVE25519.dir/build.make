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
include src/curve25519/CMakeFiles/CURVE25519.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/curve25519/CMakeFiles/CURVE25519.dir/compiler_depend.make

# Include the progress variables for this target.
include src/curve25519/CMakeFiles/CURVE25519.dir/progress.make

# Include the compile flags for this target's objects.
include src/curve25519/CMakeFiles/CURVE25519.dir/flags.make

src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/decaf.c.o: src/curve25519/CMakeFiles/CURVE25519.dir/flags.make
src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/decaf.c.o: src/GENERATED/c/curve25519/decaf.c
src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/decaf.c.o: src/curve25519/CMakeFiles/CURVE25519.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/decaf.c.o"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/decaf.c.o -MF CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/decaf.c.o.d -o CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/decaf.c.o -c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/curve25519/decaf.c

src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/decaf.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/decaf.c.i"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/curve25519/decaf.c > CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/decaf.c.i

src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/decaf.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/decaf.c.s"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/curve25519/decaf.c -o CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/decaf.c.s

src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/elligator.c.o: src/curve25519/CMakeFiles/CURVE25519.dir/flags.make
src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/elligator.c.o: src/GENERATED/c/curve25519/elligator.c
src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/elligator.c.o: src/curve25519/CMakeFiles/CURVE25519.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/elligator.c.o"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/elligator.c.o -MF CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/elligator.c.o.d -o CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/elligator.c.o -c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/curve25519/elligator.c

src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/elligator.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/elligator.c.i"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/curve25519/elligator.c > CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/elligator.c.i

src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/elligator.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/elligator.c.s"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/curve25519/elligator.c -o CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/elligator.c.s

src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/scalar.c.o: src/curve25519/CMakeFiles/CURVE25519.dir/flags.make
src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/scalar.c.o: src/GENERATED/c/curve25519/scalar.c
src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/scalar.c.o: src/curve25519/CMakeFiles/CURVE25519.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/scalar.c.o"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/scalar.c.o -MF CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/scalar.c.o.d -o CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/scalar.c.o -c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/curve25519/scalar.c

src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/scalar.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/scalar.c.i"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/curve25519/scalar.c > CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/scalar.c.i

src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/scalar.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/scalar.c.s"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/curve25519/scalar.c -o CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/scalar.c.s

src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/eddsa.c.o: src/curve25519/CMakeFiles/CURVE25519.dir/flags.make
src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/eddsa.c.o: src/GENERATED/c/curve25519/eddsa.c
src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/eddsa.c.o: src/curve25519/CMakeFiles/CURVE25519.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/eddsa.c.o"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/eddsa.c.o -MF CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/eddsa.c.o.d -o CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/eddsa.c.o -c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/curve25519/eddsa.c

src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/eddsa.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/eddsa.c.i"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/curve25519/eddsa.c > CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/eddsa.c.i

src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/eddsa.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/eddsa.c.s"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/GENERATED/c/curve25519/eddsa.c -o CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/eddsa.c.s

src/curve25519/CMakeFiles/CURVE25519.dir/decaf_tables.c.o: src/curve25519/CMakeFiles/CURVE25519.dir/flags.make
src/curve25519/CMakeFiles/CURVE25519.dir/decaf_tables.c.o: ../../src/curve25519/decaf_tables.c
src/curve25519/CMakeFiles/CURVE25519.dir/decaf_tables.c.o: src/curve25519/CMakeFiles/CURVE25519.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object src/curve25519/CMakeFiles/CURVE25519.dir/decaf_tables.c.o"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/curve25519/CMakeFiles/CURVE25519.dir/decaf_tables.c.o -MF CMakeFiles/CURVE25519.dir/decaf_tables.c.o.d -o CMakeFiles/CURVE25519.dir/decaf_tables.c.o -c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/curve25519/decaf_tables.c

src/curve25519/CMakeFiles/CURVE25519.dir/decaf_tables.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/CURVE25519.dir/decaf_tables.c.i"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/curve25519/decaf_tables.c > CMakeFiles/CURVE25519.dir/decaf_tables.c.i

src/curve25519/CMakeFiles/CURVE25519.dir/decaf_tables.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/CURVE25519.dir/decaf_tables.c.s"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/curve25519 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=aarch64-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/curve25519/decaf_tables.c -o CMakeFiles/CURVE25519.dir/decaf_tables.c.s

CURVE25519: src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/decaf.c.o
CURVE25519: src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/elligator.c.o
CURVE25519: src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/scalar.c.o
CURVE25519: src/curve25519/CMakeFiles/CURVE25519.dir/__/GENERATED/c/curve25519/eddsa.c.o
CURVE25519: src/curve25519/CMakeFiles/CURVE25519.dir/decaf_tables.c.o
CURVE25519: src/curve25519/CMakeFiles/CURVE25519.dir/build.make
.PHONY : CURVE25519

# Rule to build all files generated by this target.
src/curve25519/CMakeFiles/CURVE25519.dir/build: CURVE25519
.PHONY : src/curve25519/CMakeFiles/CURVE25519.dir/build

src/curve25519/CMakeFiles/CURVE25519.dir/clean:
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/curve25519 && $(CMAKE_COMMAND) -P CMakeFiles/CURVE25519.dir/cmake_clean.cmake
.PHONY : src/curve25519/CMakeFiles/CURVE25519.dir/clean

src/curve25519/CMakeFiles/CURVE25519.dir/depend:
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/curve25519 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/curve25519 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/arm64-v8a/src/curve25519/CMakeFiles/CURVE25519.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/curve25519/CMakeFiles/CURVE25519.dir/depend
