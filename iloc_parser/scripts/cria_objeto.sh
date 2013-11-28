#!/bin/bash
# script to create object file iloc_parser.o

DIR=/home/gandersson/code/compiladores

cd $DIR/iloc_parser/build

flex --prefix=ilocparser ../scanner.l

bison -v --defines=parser.h --output=parser.c ../parser.y 

gcc  -c ../src/iloc_parser.c lex.ilocparser.c parser.c ../../src/iks_list.c  -I../include/ -I../../include/ -lfl

