#!/usr/bin/env bash

set -euxo pipefail

RED='\033[0;31m'
NC='\033[0m' # No Color

###############################################################################
#### Configuration / Variables Setup
###############################################################################

### Detect Target
ARCH="$(arch)"
OS="$(uname)"
PLATFORM=""
if command -v lsb_release; then
  PLATFORM="$(lsb_release -is)"
fi

NPROC=""
if command -v nproc; then
  NPROC="$(nproc)"
else
  NPROC="$(sysctl -n hw.logicalcpu)"
fi

install_packages_apt() {
  ### Detect Ubuntu distro
  ### Kitware/Cmake repo only supports Ubuntu LTS versions
  if [ "$PLATFORM" = "Ubuntu" ]; then
    version="$(lsb_release -sc)"
    if [ $version != "focal" ] && [ $version != "bionic" ] && [ $version != "xenial" ]; then
      # Non-LTS version
      echo -e "${RED}Non-LTS version Ubuntu detected!${NC}"
      while true; do
        read -p "Do you wish to continue with 20.04 Focal? [y/n] " yn
        case $yn in
        [Yy]*)
          version="focal"
          break
          ;;
        [Nn]*) exit -1 ;;
        *) echo "Please answer yes or no." ;;
        esac
      done
    fi
  fi # $PLATFORM = Ubuntu

  ###############################################################################
  #### Installation
  ###############################################################################

  sudo apt update -qq
  sudo apt install -qq -y git python3-dev libconfig++-dev libcurl4 libcurl4-openssl-dev

  ### Install CMake (latest)
  if [ "$PLATFORM" = "Ubuntu" ]; then
    # Link: https://apt.kitware.com/
    sudo apt-get -y -qq install apt-transport-https ca-certificates gnupg software-properties-common wget

    wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null

    echo "deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ $(lsb_release -sc) main" | sudo tee /etc/apt/sources.list.d/kitware.list >/dev/null
    sudo apt update -qq
  fi

  sudo apt install -qq -y cmake

  ### Install g++-9
  ### Only specify version 9 on Ubuntu, since RPi doesn't have such a latest support
  if [ "$PLATFORM" = "Ubuntu" ]; then
    sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
    sudo apt update -qq
    sudo apt install -qq -y g++-9
  else
    sudo apt install -qq -y g++
  fi

  # This fix "no CMAKE_CXX_COMPILER" error on freshly installed Ubuntu
  sudo apt install -qq -y build-essential
}

install_packages_brew() {
  echo 'Installing toolchain dependencies...'

  # add build tools
  brew install cmake libconfig curl

  # check for gcc for simulation
  if ! command -v g++; then
    echo 'warning: clang/g++ needed for compilation'
    echo 'warning: please install Command Line Tools from https://developer.apple.com/download/more/'
    exit -1
  fi
}

install_packages_source() {
  echo 'source not supported currently' && exit
}

install_packages() {
  PM=source
  if command -v apt-get; then
    PM=apt
  elif command -v brew; then
    PM=brew
  fi
  install_packages_$PM
}

build_dependency() {

  ### Pull all dependencies
  git submodule update --init --recursive

  ### Install libdecaf for Sieve cryptography
  pushd libs/ed448goldilocks-code
  rm -rf build
  mkdir build
  cd build
  cmake -DPYTHON_EXECUTABLE=$(which python3) ..
  make
  sudo make install

  # Fix library include relative path issue
  cd /usr/local/include
  sudo mv decaf decaf-temp
  sudo mv decaf-temp/decaf ./
  sudo mv decaf-temp/* ./
  sudo rm -r decaf-temp
  popd

  ### Install libsodium for secret-key cryptography
  pushd libs
  tar -zxvf libsodium-1.0.18.tar.gz
  cd libsodium-1.0.18

  ./configure
  make -j"$NPROC" && make check
  sudo make install
  popd

  ### Install CryptoPP for Shamis secret sharing
  pushd libs
  tar -zxvf CRYPTOPP_8_5_0.tar.gz
  cd cryptopp-CRYPTOPP_8_5_0/

  make -j"$NPROC" libcryptopp.a libcryptopp.so
  sudo make install PREFIX=/usr/local
  popd

  #   echo '
  # # Add custom local c++ library to LD_LIBRARY_PATH
  # #configure environment variables
  # LIBTOT_PREFIX=/usr/local #the prefix where you installed libtot
  # export C_INCLUDE_PATH=$C_INCLUDE_PATH:$LIBTOT_PREFIX/include
  # export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:$LIBTOT_PREFIX/include
  # export LIBRARY_PATH=$LIBRARY_PATH:$LIBTOT_PREFIX/lib
  # export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$LIBTOT_PREFIX/lib

  # ' >>~/.bashrc

  #   echo "export TOT_SOURCE_PATH=${PWD}" >>~/.bashrc

  ### Install flatbuffers
  pushd libs/flatbuffers

  cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release

  totalm=$(free -m | awk '/^Mem:/{print $2}')
  if [[ $totalm -gt 4096 ]]; then
    make -j"$NPROC"
  else
    make
  fi
  popd

  # Link flatbuffers compiler
  ln -visf libs/flatbuffers/flatc flatc

  # Copy flatbuffers headers to include/
  pushd include
  cp -R ../libs/flatbuffers/include/flatbuffers ./
  popd
}

post_install_setup() {
  ###############################################################################
  #### Post Installation Configuration
  ###############################################################################

  ### Generate libtot flatbuffer headers
  ./bin/compile_flatbuffers_models.sh --cpp -o include/libtot/

  ### Link default user config file to home

  ln -vis "$(pwd)"/include/libtot/libtot_user_config.cfg ~/libtot_user_config.cfg

  testSize=(10K 100K 1M 10M 100M)
  for i in "${testSize[@]}"; do
    ./bin/generate_test_file.sh "$i"
  done
}

main() {
  install_packages
  echo 'package installation completed successfully.'

  build_dependency
  echo 'dependency build completed successfully.'

  post_install_setup
  echo 'post installation configuration completed successfully.'
}

main
