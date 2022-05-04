# TEO: Ephemeral Ownership for IoT Devices to Provide Granular Data Control
- [TEO: Ephemeral Ownership for IoT Devices to Provide Granular Data Control](#teo-ephemeral-ownership-for-iot-devices-to-provide-granular-data-control)
  - [Setup](#setup)
  - [Hardware and Platform Requirements](#hardware-and-platform-requirements)
  - [Formal Protocol Verification Models](#formal-protocol-verification-models)
    - [Docker Image Build](#docker-image-build)
    - [Perform Model Verification](#perform-model-verification)
      - [Reference Models](#reference-models)
      - [Compile Models](#compile-models)
      - [Run Verification](#run-verification)
      - [Interpreting Results](#interpreting-results)
  - [libteo: C++ Library and Linux Demo Apps](#libteo-c-library-and-linux-demo-apps)
    - [Dependency](#dependency)
    - [Setup](#setup-1)
    - [Build](#build)
    - [CMake configuration options](#cmake-configuration-options)
    - [Command Line Test Apps](#command-line-test-apps)
      - [Basic Functionality](#basic-functionality)
      - [Manual Exploration](#manual-exploration)
  - [Android Demo App](#android-demo-app)
    - [Requirement](#requirement)
    - [Project Import](#project-import)
    - [Why You Need Two (or more) Phones](#why-you-need-two-or-more-phones)
    - [Functionalities and Demo](#functionalities-and-demo)

This repository contains source code artifacts for the paper TEO: Ephemeral Ownership for IoT Devices to Provide Granular Data Control. It consists of three components:
- Formal protocol models
- Core TEO library and CLI
- Android demo app

Please refer to the table of content for each individual parts.

## Setup

We include several git submodules with this repo. To download this repo, please execute the following command:
```bash
git clone https://github.com/synergylabs/TEO-release
cd TEO-release
git submodule update --init --recursive
```

## Hardware and Platform Requirements

Each artifact compoenent has its own list of requirements:
- ProVerif models: Docker
- libteo: Ubuntu 20.04.4 LTS
- Android demo app: with a complete setup including 2x Android phones (tested on Nexus 5X, Android 8.1), Raspberry Pi 4 (as TEO device), Ubuntu 20.04.4 LTS (as Storage and data accessor). Moreover, make sure all devices are connected to the same local network/subnet. 

## Formal Protocol Verification Models

The model code for protocol verification is located in [proverif/](proverif/), which is built for the verification tool ProVerif ([link](https://bblanche.gitlabpages.inria.fr/proverif/)).

### Docker Image Build

We provide a Dockerfile to help with installing ProVerif and other dependencies. To build the docker image, run
```bash
# cd TEO-release/
docker build -t zhanghan177/teo .
```

### Perform Model Verification

After building the docker image, you can launch it by running
```bash
docker run -it --rm zhanghan177/teo
# This leads to the following command prompt
# The following commands in this section assumes you are running inside this container
opam@<container-id>:/app$ cd proverif
opam@<container-id>:/app/proverif$ 
```

#### Reference Models

The base model template is at [proverif/teo-template.pv](proverif/teo-template.pv).

#### Compile Models

To compile, you can set the number of users as:
```bash
python3 build.py 
# or to specify multiple users (note: we do not recommend setting numUsers > 3 as verification can take a long time):
python3 build.py numUsers=<x>
```

The output model will be in `teo-compiled.pv`.

#### Run Verification

Assuming you've already compiled the model at `teo-compiled.pv`, you can check properties with the following command:
```bash
./verify.sh
```

#### Interpreting Results

Some of the verification results are expected to be `false`, while others are supposed to be `true`. As a rule of thumb, injective correspondence queries (in the form of `Query inj-event(A) => inj-event(B)`) and data secrecy queries (e.g., `Query not attacker_p2(privateUserData[])`) are expected to be `true`, while reachability queries (e.g., `Query not (event(A) && event(B))`) are to be `false` (i.e., all events are reachable because of the double negation).

## libteo: C++ Library and Linux Demo Apps

For detailed instruction, please refer to https://github.com/synergylabs/libteo.

The following instructions assume you are under the [libteo/](libteo/) subfolder.

### Dependency

The following dependencies are included in the [setup script](#setup). We are enumerating them just for your reference. No need to take any action if you plan to use our provided setup script.

- `CryptoPP`: Need this library for Shamir secret sharing.
Install CryptoPP with CMake: https://www.cryptopp.com/wiki/CMake
If you install it manually, don't forget to run `make install` before
linking the library in this project's CMake.
- `libdecaf`: This library provides full implementation of Ed448 and support inverse lookup of elligator, i.e. translating a hash to an EC point and vice versa. https://sourceforge.net/p/ed448goldilocks/code/ci/master/tree/
- `libsodium`: Better crypto library than CryptoPP. https://github.com/jedisct1/libsodium


### Setup

Instructions on how to set up on a fresh Ubuntu machine.

- Run `./bin/setup.sh`
  - Must use **GCC-9**, as one of the dependency (json library) doesn't play well with GCC-10.

- **[Additional step for storage server]**
Run `./bin/setup_storage.sh`
  - You need to install additional dependencies if you want to build storage module (hence deploy the storage server on targeted platforms).

### Build

- **[Optional]** Run `./bin/compile_flatbuffers_models.sh --cpp -o include/teo/` to generate the flatbuffer files for message format. (This step is included in the [setup](#setup) script.)
- Run `cmake -B build -S .` to generate a buildsystem and then run the actual build command `cmake --build build`. 

### CMake configuration options

Pass these options to CMake configuration command, e.g.

| CMake option | Values | Description |
| ------------ | ------ | ----------- |
| TEO_EXTENDED_TESTS | ON / ***OFF*** | Run additional tests (Please leave off, deprecated) |
| TEO_STANDALONE_APP | ***ON*** / OFF | Build standard Linux app (instead of Android native libraries) |
| TEO_STORAGE_MODULE | ***ON*** / OFF | Build storage module for third-party storage server |
| TEO_DEMO_APPS | ***ON*** / OFF | Build apps for demonstration |
| TEO_BLUETOOTH_BEACON | ON / ***OFF*** | Enable bluetooth beacons for proximity keep-alive |
| JSON_BuildTests | ***ON*** / OFF | [Third-party] JSON library unit tests (leave on if you care or want to make sure library works) |

### Command Line Test Apps

#### Basic Functionality

Run the simple unit test:
```bash
# terminal 1
./build/apps/storage

# terminal 2: collect storage server information from above
./build/apps/app <storage-ip> <storage-port>
```
This standalone test app demonstrates some basic TEO functinalities and crypto primitives. You can check out its implementation at 

#### Manual Exploration

It is important that you start the following terminals/sessions/programs in the correct sequence. However, you do have some flexibility for exploration once the system is up and running (once user becomes an ephemeral owner).

1. Start the storage server.
   ```bash
   # terminal 1
   ./build/apps/storage
   # Collect storage server's info
   teo-storage> info 
   ```
2. Start the device program.
   ```bash
   # terminal 2
   ./build/apps/device <storage-ip> <storage-port>
   # Collect device's info for **admin**
   teo-device> info admin
   ```
3. Start the admin program.
    ```bash
    # terminal 3
    ./build/apps/admin <storage-ip> <storage-port> <device-pubkey> <device-secret>
    # Attempt to initialize the targeted device
    teo-admin> initDevice
    ```
4. Acquire pre-auth token and claim the device
    ```bash 
    # Obtain necessary information for user claim process
    # back in terminal 2
    teo-device> info user
    # terminal 4
    ./build/apps/user <storage-ip> <storage-port> <admin-pubkey> <device-pubkey>
    # Acquire pre-auth token
    teo-user> preauth
    # back in terminal 3: admin needs approve this
    teo-admin> Do you want to grant this user pre-auth token? [y/n]: # type yes or no
    # back in termial 4
    teo-user> # you should see confirmation message of pre-auth approval here
    # Next, claim the device
    teo-user> claimDevice
    ```
5. Store data
    ```bash 
    # on terminal 2
    # we provide two ways to store data (for tests)
    teo-device> test  # stores a default Hello World file
    # or
    teo-device> store <full-path-to-file>
    # After successfully storing the data, keep the UUID from terminal output as you will need it later
    ```
6. Request access 
    ```bash
    # terminal 5
    ./build/apps/accessor <storage-port> <metadata_UUID>
    teo-accessor> request <metadata_UUID>
    # go back to terminal 4
    teo-user> Do you want to grant this data access? [y/n]: # type yes or no
    # go back to terminal 5 and check the output
    ```
7. Re-encryption
    ```bash
    # at terminal 5 (or a new one) 
    # set reps (n) > 1, as subsequent access will re-use cached key 
    ./build/apps/accessor <storage-port> <metadata_UUID>
    teo-accessor> request <metadata_UUID>
    # Whenever you want to revoke accessor's access, go to terminal 4
    teo-user> reencrypt <metadata_UUID>
    # afterwards, accessor's subsequent access will fail
    teo-accessor> retryFromCache <metadata_UUID>
    ```


## Android Demo App

The subfolder [android/](android/) contains Android Studio project source code for the TEO mobile app. 

### Requirement

Please download the latest Android Studio and follow the instruction to enable its NDK toolset.
- https://developer.android.com/studio
- https://developer.android.com/ndk

### Project Import

You should be able to directly open the `TEO-release/android` subfolder from Android Studio. Then you can build and install TEO mobile APK to your test phones.

### Why You Need Two (or more) Phones
Currently, there is a bug preventing you from using the same phone for both admin and user modes. The pre-auth request notification won't be delievered properly. As a result, you need to have one dedicated phone in Admin mode, plus additional phones for other users. If you can reproduce this bug and have any idea how to fix it, feel free to let me know or create a PR! 

### Functionalities and Demo

The basic functionality and workflow are highly similar to the Linux command-line programs ([Command Line Test Apps](#command-line-test-apps)). We have refactored our mobile app used for evaluation to accomendate for the upcoming demo submission. 

One key notice is that this demo app relies on QR code scanning (rather than user input and manual typing) to pass information around (e.g., TEO devices). The user interface is fairly straightforward so you should be able to figure out when you need to scan the code (and which one). 