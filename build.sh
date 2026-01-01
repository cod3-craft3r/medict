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
cmake --build . --clean-first || { echo "Build Error(s)"; exit 1; }
cd ../

# this worked for me when there was NO actual SDL code; only a header was being included.
# g++ -g -std=c++20 src/*.cc -I include/ -o medict-dev -lstdc++fs -lSDL2
