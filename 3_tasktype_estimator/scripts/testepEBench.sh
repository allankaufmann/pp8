#!/bin/bash

cd ..
echo "$PWD"
cd epEBench/bin/Release
./epebench -m dadd -t 1 -a 1 -n 1
mv epebench_loadlog.txt epebench_dadd.log
cd ../../..