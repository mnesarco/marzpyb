#! /bin/bash

BUILD="./build"


compile() {
    local COMP=$1
    if [ -d "$BUILD" ]; then
        rm -rf "$BUILD"
    fi
    mkdir -p "$BUILD"
    cd "$BUILD" \
        && cmake $COMP -G Ninja -DCMAKE_BUILD_TYPE=Release .. \
        && time ninja test_pyarguments \
        && time ninja test_pycxx_arguments \
        && time ninja compile_benchmark \
        && cd ..
}

compile "-DCMAKE_CXX_COMPILER=/usr/bin/clang++-20"
compile ""
