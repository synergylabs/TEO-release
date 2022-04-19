#!/usr/bin/env bash
set -euxo pipefail

HELP=false
LANGUAGE=""
OUTPUT_DIR="include/teo/"

USAGE="
        -c|--cpp|-j|--java  Set output language (Java or C++).
        -o <output_dir>     Set output directory [Default: ${OUTPUT_DIR}].

DESCRIPTION
  Execute Flatbuffers compiler to generate files for target client language.

OPTIONS
  Nil.
"

# Detect appropriate executables for getopt, and install if necessary.
unameOut="$(uname -s)"
GETOPT=""
case "${unameOut}" in
Linux*)
  machine=Linux
  GETOPT="getopt"
  ;;
Darwin*)
  machine=Mac
  GETOPT="$(brew --prefix gnu-getopt)/bin/getopt"
  ;;
CYGWIN*)
  machine=Cygwin
  echo "Unsupport platform -- ${machine}"
  exit 1
  ;;
MINGW*)
  machine=MinGw
  echo "Unsupport platform -- ${machine}"
  exit 1
  ;;
*)
  echo "UNKNOWN MACHINE:${unameOut}"
  exit 1
  ;;
esac

if ! command -v $GETOPT; then
  echo "${GETOPT} could not be found, installing right now..."
  case $machine in
  Linux*)
    sudo apt update -qq
    sudo apt install -y -qq $GETOPT
    ;;
  Mac*)
    brew update
    brew install gnu-getopt
    ;;
  esac
fi

# Process command line options
OPTS=$(${GETOPT} -o hcjo: --long help,cpp,java -n 'parse-options' -- "$@")
if [ $? != 0 ]; then
  echo "Failed parsing options." >&2
  exit 1
fi

eval set -- "$OPTS"

while true; do
  case "$1" in
  -h | --help)
    HELP=true
    shift
    ;;
  -c | --cpp)
    LANGUAGE="--cpp"
    shift
    ;;
  -j | --java)
    LANGUAGE="--java"
    shift
    ;;
  -o)
    OUTPUT_DIR="$2"
    shift
    shift
    ;;
  --)
    shift
    break
    ;;
  *) break ;;
  esac
done

if [[ -z "${LANGUAGE}" || -z "${OUTPUT_DIR}" ]]; then
  echo "Missing arguments"
  HELP=true
fi

if [ "$HELP" = true ]; then
  echo -e "${USAGE}"
  exit 0
fi

case "${LANGUAGE}" in
"--cpp")
  ./flatc "${LANGUAGE}" -o "${OUTPUT_DIR}/Message" src/Message/*.fbs
  ./flatc "${LANGUAGE}" -o "${OUTPUT_DIR}/DataStore" src/DataStore/*.fbs
  ;;
"--java")
  ./flatc "${LANGUAGE}" -o "${OUTPUT_DIR}" src/**/*.fbs
  ;;
esac
exit 0
