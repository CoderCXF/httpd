#!/bin/bash
work_path=$(dirname $0)
echo ${work_path}
if [ ! -d build/ ]
then
    mkdir build
fi

cd build && cmake ..
make
