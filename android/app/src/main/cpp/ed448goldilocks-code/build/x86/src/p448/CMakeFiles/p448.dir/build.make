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
include src/p448/CMakeFiles/p448.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/p448/CMakeFiles/p448.dir/compiler_depend.make

# Include the progress variables for this target.
include src/p448/CMakeFiles/p448.dir/progress.make

# Include the compile flags for this target's objects.
include src/p448/CMakeFiles/p448.dir/flags.make

src/p448/CMakeFiles/p448.dir/arch_32/f_impl.c.o: src/p448/CMakeFiles/p448.dir/flags.make
src/p448/CMakeFiles/p448.dir/arch_32/f_impl.c.o: ../../src/p448/arch_32/f_impl.c
src/p448/CMakeFiles/p448.dir/arch_32/f_impl.c.o: src/p448/CMakeFiles/p448.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/p448/CMakeFiles/p448.dir/arch_32/f_impl.c.o"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/p448 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=i686-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/p448/CMakeFiles/p448.dir/arch_32/f_impl.c.o -MF CMakeFiles/p448.dir/arch_32/f_impl.c.o.d -o CMakeFiles/p448.dir/arch_32/f_impl.c.o -c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/p448/arch_32/f_impl.c

src/p448/CMakeFiles/p448.dir/arch_32/f_impl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/p448.dir/arch_32/f_impl.c.i"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/p448 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=i686-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/p448/arch_32/f_impl.c > CMakeFiles/p448.dir/arch_32/f_impl.c.i

src/p448/CMakeFiles/p448.dir/arch_32/f_impl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/p448.dir/arch_32/f_impl.c.s"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/p448 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=i686-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/p448/arch_32/f_impl.c -o CMakeFiles/p448.dir/arch_32/f_impl.c.s

src/p448/CMakeFiles/p448.dir/f_arithmetic.c.o: src/p448/CMakeFiles/p448.dir/flags.make
src/p448/CMakeFiles/p448.dir/f_arithmetic.c.o: ../../src/p448/f_arithmetic.c
src/p448/CMakeFiles/p448.dir/f_arithmetic.c.o: src/p448/CMakeFiles/p448.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object src/p448/CMakeFiles/p448.dir/f_arithmetic.c.o"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/p448 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=i686-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/p448/CMakeFiles/p448.dir/f_arithmetic.c.o -MF CMakeFiles/p448.dir/f_arithmetic.c.o.d -o CMakeFiles/p448.dir/f_arithmetic.c.o -c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/p448/f_arithmetic.c

src/p448/CMakeFiles/p448.dir/f_arithmetic.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/p448.dir/f_arithmetic.c.i"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/p448 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=i686-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/p448/f_arithmetic.c > CMakeFiles/p448.dir/f_arithmetic.c.i

src/p448/CMakeFiles/p448.dir/f_arithmetic.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/p448.dir/f_arithmetic.c.s"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/p448 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=i686-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/p448/f_arithmetic.c -o CMakeFiles/p448.dir/f_arithmetic.c.s

src/p448/CMakeFiles/p448.dir/__/GENERATED/c/p448/f_generic.c.o: src/p448/CMakeFiles/p448.dir/flags.make
src/p448/CMakeFiles/p448.dir/__/GENERATED/c/p448/f_generic.c.o: src/GENERATED/c/p448/f_generic.c
src/p448/CMakeFiles/p448.dir/__/GENERATED/c/p448/f_generic.c.o: src/p448/CMakeFiles/p448.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object src/p448/CMakeFiles/p448.dir/__/GENERATED/c/p448/f_generic.c.o"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/p448 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=i686-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/p448/CMakeFiles/p448.dir/__/GENERATED/c/p448/f_generic.c.o -MF CMakeFiles/p448.dir/__/GENERATED/c/p448/f_generic.c.o.d -o CMakeFiles/p448.dir/__/GENERATED/c/p448/f_generic.c.o -c /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/GENERATED/c/p448/f_generic.c

src/p448/CMakeFiles/p448.dir/__/GENERATED/c/p448/f_generic.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/p448.dir/__/GENERATED/c/p448/f_generic.c.i"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/p448 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=i686-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/GENERATED/c/p448/f_generic.c > CMakeFiles/p448.dir/__/GENERATED/c/p448/f_generic.c.i

src/p448/CMakeFiles/p448.dir/__/GENERATED/c/p448/f_generic.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/p448.dir/__/GENERATED/c/p448/f_generic.c.s"
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/p448 && /Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang --target=i686-none-linux-android23 --gcc-toolchain=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64 --sysroot=/Users/hanzhang/Library/Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/GENERATED/c/p448/f_generic.c -o CMakeFiles/p448.dir/__/GENERATED/c/p448/f_generic.c.s

p448: src/p448/CMakeFiles/p448.dir/arch_32/f_impl.c.o
p448: src/p448/CMakeFiles/p448.dir/f_arithmetic.c.o
p448: src/p448/CMakeFiles/p448.dir/__/GENERATED/c/p448/f_generic.c.o
p448: src/p448/CMakeFiles/p448.dir/build.make
.PHONY : p448

# Rule to build all files generated by this target.
src/p448/CMakeFiles/p448.dir/build: p448
.PHONY : src/p448/CMakeFiles/p448.dir/build

src/p448/CMakeFiles/p448.dir/clean:
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/p448 && $(CMAKE_COMMAND) -P CMakeFiles/p448.dir/cmake_clean.cmake
.PHONY : src/p448/CMakeFiles/p448.dir/clean

src/p448/CMakeFiles/p448.dir/depend:
	cd /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/src/p448 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/p448 /Users/hanzhang/Research/Transient-IoT-Ownership/tot_mobile_native/app/src/main/cpp/ed448goldilocks-code/build/x86/src/p448/CMakeFiles/p448.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/p448/CMakeFiles/p448.dir/depend

