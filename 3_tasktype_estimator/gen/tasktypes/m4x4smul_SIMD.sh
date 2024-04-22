#!/bin/bash
CORES=$1
if [ -z "$CORES" ]
then
CORES=1
fi
echo Anzahl ist $CORES
cd ..
cd epEBench/bin/Release
./epebench -m m4x4smul_SIMD -t 5 -n $CORES -u 100 
chmod a+w epebench_loadlog.txt
mv epebench_loadlog.txt epebench_m4x4smul_SIMD.log
cd ../../..