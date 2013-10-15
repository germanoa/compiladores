#!/bin/bash

BIN=build/main
INPUT=$1
VALGRIND_OPTS=" --track-origins=yes --leak-check=full --show-reachable=yes -v"

scripts/compila.sh
valgrind $VALGRIND_OPTS $BIN < $INPUT
