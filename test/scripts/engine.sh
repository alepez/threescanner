#!/bin/bash

SCRIPT_DIR="$( cd $( dirname ${BASH_SOURCE}) && pwd )"
CURR_DIR="$( cd ${SCRIPT_DIR}/../mocks && pwd )"
BIN_DIR="$( cd ${SCRIPT_DIR}/../../build/ && pwd )"

export PATH="${PATH}:${BIN_DIR}"

cd "${CURR_DIR}"

${BIN_DIR}/engine
