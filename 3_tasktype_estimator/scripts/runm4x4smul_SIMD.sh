#!/bin/bash

cd ..
echo "$PWD"
cd epEBench/bin/Release
./epebench -m m4x4smul_SIMD -t 1 -a 1 -n 1
mv epebench_loadlog.txt epebench_m4x4smul_SIMD.log
cd ../../..