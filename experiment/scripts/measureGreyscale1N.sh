#!/bin/bash
cd ..
cd epEBench/bin/Release
./epebench -m greyscale -t 2 -a 1 -n 1
mv epebench_loadlog.txt epebench_greyscale.log
cd ../../..
