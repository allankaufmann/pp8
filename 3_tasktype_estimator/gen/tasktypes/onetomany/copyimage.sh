#!/bin/bash
CORES=$1
if [ -z "$CORES" ]
then
CORES=1
fi
echo Anzahl ist $CORES
cd ..
cd epEBench/bin/Release
./epebench -m copyimage -t 5 -n $CORES -u 100 
chmod a+w epebench_loadlog.txt
mv epebench_loadlog.txt epebench_copyimage.log
cd ../../..