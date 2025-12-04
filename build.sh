#!/bin/bash

if [[ "$1" == "clean" ]]; then
    rm -rf "./build"
    exit 0
elif [[ "$1" == "rebuild" ]]; then
    rm -rf "./build"
fi

# Create build directory if it doesn't exist
mkdir -p build
cd build
cmake .. || { echo "CMake Error(s)"; exit 1; }
make

