#!/bin/bash

if [ "$1" = "release" ]
then
    ./build/release/bin/compute_tests 
else
    ./build/debug/bin/compute_tests 
fi