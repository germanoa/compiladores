#!/bin/bash

CMAKEFLAGS=" -g3 -DCMAKE_BUILD_TYPE=Debug "

cd build
cmake $CMAKEFLAGS ..
make
