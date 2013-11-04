#!/bin/bash

CMAKEFLAGS=" -g3 -DCMAKE_BUILD_TYPE=Debug "

rm -rf build
mkdir build
cd build
cmake $CMAKEFLAGS ..
make
cd ..
