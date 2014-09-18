#!/bin/bash

ROOT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/../" && pwd )";
THIRD_PARTY_DIR="$( cd "${ROOT_DIR}/third_party" && pwd )";

mkdir -p "${THIRD_PARTY_DIR}/tmp";
mkdir -p "${THIRD_PARTY_DIR}/lib";
mkdir -p "${THIRD_PARTY_DIR}/include";

cd "${THIRD_PARTY_DIR}/tmp";

wget https://github.com/cppformat/cppformat/archive/master.zip -O cppformat.zip
unzip cppformat.zip
rm -f cppformat.zip
cd cppformat-master
cmake .
make
cp libformat.a libgmock.a ../../lib
cp format.h ../../include
