#!/usr/bin/env bash

set -euxo pipefail

LIBTOT_DIR="${HOME}/Research/Transient-IoT-Ownership/libtot"

cd app/src/main/cpp

# Replace *.cpp with src/ folder files
array=()
while IFS=  read -r -d $'\0'; do
    array+=("$REPLY")
done < <(find . -name "*.cpp" -type l -print0)

for f in ${array[@]+"${array[@]}"}
do
  ln -visf "${LIBTOT_DIR}/src/${f}" "${f}"
done


# Replace *.hpp with include/ folder files
array=()
while IFS=  read -r -d $'\0'; do
    array+=("$REPLY")
done < <(find . -name "*.hpp" -type l -print0)

for f in ${array[@]+"${array[@]}"}
do
  ln -visf "${LIBTOT_DIR}/include/${f}" "${f}"
done

ln -visf "${LIBTOT_DIR}/include/flatbuffers" include/flatbuffers
ln -visf "${LIBTOT_DIR}/include/libtot/DataStore" libtot/DataStore
ln -visf "${LIBTOT_DIR}/include/libtot/Message" libtot/Message
