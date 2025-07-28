#!/bin/bash

cmake --build build
cd build || exit
./runme
