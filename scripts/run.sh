#!/bin/bash

if [ "$1" = "release" ]
then
    ./build/release/PROJECT_NAME_PLACEHOLDER
else
    ./build/debug/PROJECT_NAME_PLACEHOLDER
fi