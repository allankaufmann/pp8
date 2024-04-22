#!/bin/bash
cd ..
cd epEBench/bin/Release
./epebench -m vmov_SIMD -t 1 -a 1 -n 1
mv epebench_loadlog.txt epebench_vmov_SIMD.log
cd ../../..
