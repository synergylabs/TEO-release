# Transient Ownership for IoT Devices

![Compile Workflow](https://github.com/synergylabs/libtot/actions/workflows/cmake.yml/badge.svg)


## Dependency

`CryptoPP`: Need this library for Shamir secret sharing.
Install CryptoPP with CMake: https://www.cryptopp.com/wiki/CMake
You need to install this manually and `make install` before
linking the library in this project's CMake.

`libdecaf`: This library provides full implementation of Ed448 and support inverse lookup of elligator, i.e. translating a hash to an EC point and vice versa. https://sourceforge.net/p/ed448goldilocks/code/ci/master/tree/

`libsodium`: Better crypto library than CryptoPP. https://github.com/jedisct1/libsodium


## Setup

Instructions on how to set up on a fresh Ubuntu machine.

- Run `./bin/setup.sh`.

Must use **GCC-9**, as one of the dependency (json library) doesn't play well with GCC-10.

## Build

- Run `./bin/compile_flatbuffers.sh` to generate the flatbuffer files for message format.
- Run `mkdir build && cd build && cmake ..` and then run the actual build command `make`. 

## CMake configuration options

Pass these options to CMake configuration command, e.g.

```bash
cmake ..
```


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

## Changes to libdecaf

[Deprecated. Use the default libdecaf version plz.]

- Change `eligator.tmpl.c` to make from_hash_nonuniform to return hint.
- Also change `point.tmpl.h` to make same function return hint (unsigned char).
- Change `point.tmpl.hxx` to make `set_to_hash` return the hint.
