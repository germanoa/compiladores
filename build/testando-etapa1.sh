#!/bin/bash

cd src/tests/tokens

for i in `seq 0 15`; do cat input/nivel$i.tokens | ../../../build/test > nossa-saida/nivel$i.saida; done
for i in `seq 0 15`; do diff nossa-saida/nivel$i.saida saida-esperada/nivel$i.saida > diffs/nivel$i.saida ; done
