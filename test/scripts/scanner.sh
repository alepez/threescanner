#!/bin/bash

SCRIPT_DIR="$( cd $( dirname ${BASH_SOURCE}) && pwd )"
CURR_DIR="$( cd ${SCRIPT_DIR}/../mocks && pwd )"
BIN_DIR="$( cd ${SCRIPT_DIR}/../../build/ && pwd )"

export PATH="${PATH}:${BIN_DIR}"

cd "${CURR_DIR}"

function send_commands {
  (
    echo "scan"
    sleep 2
    echo "quit"
  ) | nc localhost 11601
}

( sleep 1 && send_commands ) &

${BIN_DIR}/scanner
