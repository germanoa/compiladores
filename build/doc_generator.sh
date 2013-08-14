#!/bin/bash

if [ $0 = "./doc_generator.sh" ]; then
    export PROJECT_DIR=../
    cd $PROJECT_DIR
    doxygen doc/Doxyfile
else
    echo "error. try cd PROJECT_DIR/build; ./doc_generator.sh"
fi
