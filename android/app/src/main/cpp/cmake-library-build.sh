#!/usr/bin/env bash

declare -a abis=( "arm64-v8a" "armeabi-v7a" "x86" "x86_64" )

decaf_output="ed448goldilocks-code/build"
sodium_output="libsodium-1.0.18/build"

build_decaf () {
  abi="$1"
  echo "Building decaf for: $abi"
  abi_output="$decaf_output/$abi"
  mkdir -p "$abi_output"

  pushd "$abi_output"
  cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI="$abi" -DANDROID_NATIVE_API_LEVEL=23 ../..
  make -j$(nproc)
  popd
}


build_sodium () {
  abi="$1"
  echo "Building sodium for: $abi"
  abi_output="$sodium_output/$abi"
  mkdir -p "$sodium_output"

  pushd "$abi_output"
  popd
}


for abi in "${abis[@]}"
do
  build_decaf "$abi"
#  build_sodium "$abi"
done