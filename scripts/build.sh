#!/bin/bash

target_release() {
    cd release
    cmake -DCMAKE_BUILD_TYPE=Release ../..
    make
    echo "Built target in build/release/"
    cd ../..
}

target_debug() {
    cd debug 
    cmake -DCMAKE_BUILD_TYPE=Debug ../..
    make
    echo "Built target in build/debug/"
    cd ../..
}

# Create folder for distribution
if [ "$1" = "release" ]
then
    if [ -d "$PROJECT_NAME_PLACEHOLDER" ]
    then
        rm -rf -d PROJECT_NAME_PLACEHOLDER
    fi

    mkdir -p PROJECT_NAME_PLACEHOLDER
fi

# Creates the folder for the buildaries
mkdir -p PROJECT_NAME_PLACEHOLDER 
mkdir -p PROJECT_NAME_PLACEHOLDER/assets
mkdir -p build
mkdir -p build/release
mkdir -p build/debug
cd build

# Builds target
if [ "$1" = "release" ]
then
    target_release
    cp build/release/PROJECT_NAME_PLACEHOLDER PROJECT_NAME_PLACEHOLDER/PROJECT_NAME_PLACEHOLDER
else
    target_debug
fi

cp -R assets PROJECT_NAME_PLACEHOLDER/
