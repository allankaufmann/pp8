#!/bin/bash

cd ..
echo "$PWD"
cd epEBench/bin/Release
./epebench -m vidplay -t 1 -a 1 -n 1
mv epebench_loadlog.txt vidplay.log
cd ../../..