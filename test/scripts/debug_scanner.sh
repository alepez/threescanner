#!/bin/bash

SCRIPT_DIR="$( cd $( dirname ${BASH_SOURCE}) && pwd )"
CURR_DIR="$( cd ${SCRIPT_DIR}/../mocks && pwd )"
BIN_DIR="$( cd ${SCRIPT_DIR}/../../build/ && pwd )"

cd ${CURR_DIR}

function try_gdb() {
  if [ -e "${1}" ]; then {
    gdb -x gdbinit --args "${1}"
    exit
  } fi
  return 255
}

try_gdb ${BIN_DIR}/scanner

echo "Cannot find executable";
