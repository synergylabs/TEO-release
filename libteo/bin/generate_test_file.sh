#! /usr/bin/env bash

USAGE="
    ${0}: <size>

Arguments:
- <size>: Example: 100M, 1G.
"

if [ "$#" -ne 1 ]; then
echo "$USAGE"
exit -1
fi

blockCount="$(echo ${1} | tr '[:upper:]' '[:lower:]')"
if [[ $blockCount == *k ]]
then 
    blockCount=${blockCount%"k"}
fi 

if [[ $blockCount == *m ]]
then 
    blockCount=${blockCount%"m"}
    blockCount="$(expr $blockCount '*' 1024)"
fi 

if [[ $blockCount == *g ]]
then 
    blockCount=${blockCount%"g"}
    blockCount="$(expr $blockCount '*' 1024 '*' 1024)"
fi 

# dd if=/dev/urandom of=tests/eval-"${1}"B.txt bs=1 count=0 seek="$(echo ${1} | tr '[:upper:]' '[:lower:]')"
dd if=/dev/urandom of=tests/eval-"${1}"B.txt bs=1024 count="${blockCount}"
