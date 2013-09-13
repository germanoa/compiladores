#!/bin/bash

cd src/tests/tokens

rm -f nossa-saida/*
rm -f diffs/*

for i in `seq 0 15`; do cat input/nivel$i.tokens | ../../../build/test > nossa-saida/nivel$i.saida; done
for i in `seq 0 15`; do diff nossa-saida/nivel$i.saida saida-esperada/nivel$i.saida > diffs/nivel$i.saida ; done
