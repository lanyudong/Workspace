#!/bin/bash

# setup environment
export CPU=x86_64
export OS=linux

# build
export SRC=logger
make -C ../build

# run
./../build/bin/debug/$SRC
