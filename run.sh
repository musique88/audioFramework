#!/bin/bash

cd bin
cmake -DCMAKE_BUILD_TYPE=Debug ..
make clean
make
lldb -o run AudioFramework