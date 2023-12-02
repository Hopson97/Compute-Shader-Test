#!/bin/bash

if [ "$1" = "release" ]
then
    ./build/release/bin/dsa_practice 
else
    ./build/debug/bin/dsa_practice 
fi