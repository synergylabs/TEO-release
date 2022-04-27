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
  - [Android Demo App](#android-demo-app)

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
- Android demo app: with a complete setup including 2x Android phones (tested on Nexus 5X, Android 8.1), Raspberry Pi 4 (as TEO device), Ubuntu 20.04.4 LTS (as Storage and data accessor).

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

For detailed instruction, please refer to [libteo/](https://github.com/zhanghan177/libteo).

## Android Demo App

See [android/](android/).
