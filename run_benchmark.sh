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
        && time ninja marzpyb \
        && time ninja compile_benchmark \
        && time ninja compile_benchmark2 \
        && cd ..
}

compile "-DCMAKE_CXX_COMPILER=/usr/bin/clang++-20"
compile ""
