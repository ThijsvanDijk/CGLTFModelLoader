#!/bin/bash

NOFPROC=`expr $(nproc) - 1`
rm -R build
mkdir build
cmake -B build/ -S ./
cmake --build build/ --config Release -j$NOFPROC
./build/CGLTFModelLoader