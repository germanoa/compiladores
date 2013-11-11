#! /bin/bash

echo "O arquivo de entrada é $1 "
echo "O arquivo de saída é $2"

CMAKEFLAGS=" -g3 -DCMAKE_BUILD_TYPE=Debug "

rm -rf build
mkdir build
cd build
cmake $CMAKEFLAGS
make
cd ..

build/main < $1 > $2 

dot ast_graph.dot -Tpng -o ast_graph.png
display ast_grap.png
