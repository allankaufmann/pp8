#!/bin/bash
cd ..
cd epEBench/bin/Release
./epebench -m copyimage -t 5 -a 1 -n 1
mv epebench_loadlog.txt epebench_copyimage.log
cd ../../..
