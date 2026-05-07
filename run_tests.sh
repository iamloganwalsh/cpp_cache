#!/bin/bash

set -e

echo "=============================="
echo " Cleaning build directory"
echo "=============================="

rm -rf build
mkdir build

echo "=============================="
echo " Configuring project"
echo "=============================="

cmake -S . -B build

echo "=============================="
echo " Building tests"
echo "=============================="

cmake --build build --target tests

echo "=============================="
echo " Running tests"
echo "=============================="

./build/tests 1> /dev/null