# Transient Ownership for IoT Devices

## Dependency

`CryptoPP`: Need this library for Shamir secret sharing.
Install CryptoPP with CMake: https://www.cryptopp.com/wiki/CMake
You need to install this manually and `make install` before
linking the library in this project's CMake.

`libdecaf`: This library provides full implementation of Ed448 and support inverse lookup of elligator, i.e. translating a hash to an EC point and vice versa. https://sourceforge.net/p/ed448goldilocks/code/ci/master/tree/

`libsodium`: Better crypto library than CryptoPP. https://github.com/jedisct1/libsodium


## Setup

Instructions on how to set up on a fresh Ubuntu machine.

- Run `./bin/setup.sh`

Must use **GCC-9**, as one of the dependency (json library) doesn't play well with GCC-10.

**[Additional step for storage server]**
You need to install additional dependencies if you want to build storage module (hence deploy the storage server on targeted platform).

- Run `./bin/setup_storage.sh`

## Build

- [Optional] Run `./bin/compile_flatbuffers_models.sh --cpp -o include/teo/` to generate the flatbuffer files for message format. (This step is included in the [setup](#setup) script.)
- Run `cmake -B build -S .` to generate a buildsystem and then run the actual build command `cd build && make`. 

## CMake configuration options

Pass these options to CMake configuration command, e.g.

| CMake option | Values | Description |
| ------------ | ------ | ----------- |
| TEO_EXTENDED_TESTS | ON / **OFF** | Run additional tests (Please leave off, deprecated) |
| TEO_STANDALONE_APP | **ON** / OFF | Build standard Linux app (instead of Android native libraries) |
| TEO_STORAGE_MODULE | **ON** / OFF | Build storage module for third-party storage server |
| TEO_DEMO_APPS | **ON** / OFF | Build apps for demonstration |

## Case Studies

### Motion Camera

See `cases/motion`.

### Mycroft AI Speaker Assistant

See `cases/mycroft`.

### Smart Doorlock

See `cases/doorlock`.


## Reference

Learn CMake basics: https://cliutils.gitlab.io/modern-cmake/

Reference repository: https://gitlab.com/CLIUtils/modern-cmake/tree/master/examples/extended-project

