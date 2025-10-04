#! /bin/bash

BUILD="./build"

if [ -d "$BUILD" ]; then
    rm -rf "$BUILD"
fi

mkdir "$BUILD"
cd "$BUILD"
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
time ninja test_pyarguments
time ninja test_pycxx_arguments
time ninja compile_benchmark
