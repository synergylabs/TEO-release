# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.

project("teo_mobile")

# Find all header files
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "**/*.hpp")


# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${HEADER_LIST}

        native-lib.cpp
        test_decaf.cpp
        tot-helper.cpp
        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)


#####################################
# Require libdecaf for Ed25519 crypto
#####################################
# Sets lib_src_DIR to the path of the target CMake project.
set(libdecaf_src_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ed448goldilocks-code)

# Sets lib_build_DIR to the path of the desired output directory.
set(libdecaf_build_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ed448goldilocks-code/build)

# Adds the outout of the additional CMake build as a prebuilt static
# library and names it lib_decaf
add_library(libdecaf STATIC IMPORTED)
set_target_properties(libdecaf PROPERTIES IMPORTED_LOCATION
        ${libdecaf_build_DIR}/${ANDROID_ABI}/src/libdecaf.a)
include_directories(${libdecaf_build_DIR}/${ANDROID_ABI}/src/GENERATED/include)


#####################################
# Require libsodium for general crypto
#####################################

# Link: https://github.com/robinlinden/libsodium-cmake

include(FetchContent)

FetchContent_Declare(Sodium
        GIT_REPOSITORY https://github.com/zhanghan177/libsodium-cmake.git
        )
set(SODIUM_DISABLE_TESTS ON)
FetchContent_MakeAvailable(Sodium)

######################################
## Require core TEO library
######################################
FetchContent_Declare(TEO
        GIT_REPOSITORY https://github.com/zhanghan177/libteo.git
        GIT_TAG        main
        )
set(TEO_EXTENDED_TESTS      OFF)
set(TEO_STANDALONE_APP      OFF)
set(TEO_STORAGE_MODULE      OFF)
set(TEO_DEMO_APPS           OFF)
set(TEO_BLUETOOTH_BEACON    OFF)
FetchContent_MakeAvailable(TEO)

#####################################
# Link all dependencies
#####################################

target_include_directories(native-lib PUBLIC include )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        native-lib

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}

        libdecaf
        sodium
        teo-mobile-native
        )