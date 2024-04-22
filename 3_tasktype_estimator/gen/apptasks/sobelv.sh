#!/bin/bash
CORES=$1
if [ -z "$CORES" ]
then
CORES=1
fi
echo Anzahl ist $CORES
cd ..
cd edgedetection
echo "$PWD"
./edgedetection imgfilenames $CORES sobelv
