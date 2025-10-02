#! /bin/bash

BUILD="./build"

if [ -d "$BUILD" ]; then
    rm -rf "$BUILD"
fi

mkdir "$BUILD"
cd "$BUILD"
cmake -G Ninja ..
time ninja compile_benchmark
