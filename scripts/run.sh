#!/bin/bash

cd ..

rm rubix-solver

cmake .
make -j4

./rubix-solver