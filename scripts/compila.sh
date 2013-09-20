#!/bin/bash

CMAKEFLAGS=" -DCMAKE_BUILD_TYPE=Debug "

cd build
cmake $CMAKEFLAGS ..
make
