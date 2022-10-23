#!/bin/bash

EXEC="rubix-solver"

cd ..

rm ./$EXEC

cmake .
make -j4

./$EXEC