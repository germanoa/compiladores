#!/bin/bash

CMAKEFLAGS=" -g3 -DCMAKE_BUILD_TYPE=Debug "

./scripts/cria_objeto.sh


rm -rf build
mkdir build
cd build
cmake $CMAKEFLAGS ..
make
cd ..
