#!/bin/bash
mkdir -p build
cmake -G Ninja -B ./build -DCMAKE_BUILD_TYPE=Release 
