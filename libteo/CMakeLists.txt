################################################################################################################
# HEADER Project definition.
################################################################################################################

cmake_minimum_required(VERSION 3.16)
project(libtot
        VERSION 0.1.0
        DESCRIPTION "I have no idea what I'm doing"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Create a compile_commands.json file that can be easily parsed by build tools, clang-tidy, etc.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

################################################################################################################
# HEADER Project configuration.
################################################################################################################

option(LIBTOT_EXTENDED_TESTS "Run additional tests" OFF)
option(LIBTOT_STANDALONE_APP "Build standard Linux app (instead of Android native libraries)" ON)

# propagates CMake configuration options to the compiler
function(configureProject)
    if (LIBTOT_EXTENDED_TESTS)
        add_definitions(-DLIBTOT_EXTENDED_TESTS)
    endif()
    if (LIBTOT_STANDALONE_APP)
        add_definitions(-DLIBTOT_STANDALONE_APP)
    endif()
endfunction()

configureProject()


################################################################################################################
# HEADER System info.
################################################################################################################

message(STATUS "----------------------------------------------------------------------------")
message(STATUS "-- CMake ${CMAKE_VERSION}")
message(STATUS "-- Build ${CMAKE_BUILD_TYPE} / ${CMAKE_SYSTEM_NAME}")
message(STATUS "----------------------------------------------------------------- components")
message(STATUS "-- LIBTOT_EXTENDED_TESTS      Run additional tests                ${LIBTOT_EXTENDED_TESTS}")
message(STATUS "-- LIBTOT_STANDALONE_APP      Build standard Linux app            ${LIBTOT_STANDALONE_APP}")
message(STATUS "                              (instead of Android native libraries)")
message(STATUS "----------------------------------------------------------------------------")

message(STATUS "Compiler: ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "System: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION} ${CMAKE_SYSTEM_PROCESSOR}")

function(print_sys_info QUERY_TARGET)
    cmake_host_system_information(RESULT SYS_INFO QUERY ${QUERY_TARGET})
    message(STATUS "  System ${QUERY_TARGET}: ${SYS_INFO}")
endfunction()
print_sys_info("NUMBER_OF_LOGICAL_CORES;NUMBER_OF_PHYSICAL_CORES")
print_sys_info("HOSTNAME;FQDN")
print_sys_info("AVAILABLE_VIRTUAL_MEMORY;TOTAL_VIRTUAL_MEMORY")
print_sys_info("AVAILABLE_PHYSICAL_MEMORY;TOTAL_PHYSICAL_MEMORY")
print_sys_info("IS_64BIT;HAS_IA64")
print_sys_info("HAS_FPU;HAS_MMX;HAS_MMX_PLUS")
print_sys_info("HAS_SSE;HAS_SSE2;HAS_SSE_FP;HAS_SSE_MMX")
print_sys_info("HAS_AMD_3DNOW;HAS_AMD_3DNOW_PLUS")
print_sys_info("HAS_SERIAL_NUMBER;PROCESSOR_SERIAL_NUMBER")
print_sys_info("PROCESSOR_NAME;PROCESSOR_DESCRIPTION")
print_sys_info("OS_NAME;OS_RELEASE;OS_VERSION;OS_PLATFORM")

message("----------------------------------------------------------------------------")
message("")

# https://stackoverflow.com/questions/13428910/how-to-set-the-environmental-variable-ld-library-path-in-linux
SET(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,/usr/local/lib")

# FetchContent added in CMake 3.11, downloads during the configure step
include(FetchContent)

# The compiled library code is here
add_subdirectory(src)

# The executable code is here
add_subdirectory(apps)

