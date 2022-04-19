#!/usr/bin/env bash
# Additional setup for storage node

set -euxo pipefail

###############################################################################
#### Installation
###############################################################################

sudo apt update -qq 
sudo apt upgrade -y -qq

### Install RocksDB
# Dependencies
sudo apt-get install -y libgflags-dev
sudo apt-get install -y libsnappy-dev
sudo apt-get install -y zlib1g-dev
sudo apt-get install -y libbz2-dev
sudo apt-get install -y liblz4-dev
sudo apt-get install -y libzstd-dev

sudo apt-get -y install devscripts debhelper build-essential fakeroot zlib1g-dev libbz2-dev libsnappy-dev libgflags-dev libzstd-dev


# Get repo
wait -n 
(
    cd libs 

    # Install Snappy compression library
    git clone https://github.com/google/snappy.git
    cd snappy
    git checkout 78650d1
    git submodule update --init
    mkdir build
    cd build && cmake ../ && make
    sudo make install 
    cd ../..

    # Install LevelDB
    git clone -b 1.23 --recurse-submodules https://github.com/google/leveldb.git
    cd leveldb 
    mkdir -p build && cd build
    cmake -DCMAKE_BUILD_TYPE=Release .. && sudo cmake --build . --target install
)