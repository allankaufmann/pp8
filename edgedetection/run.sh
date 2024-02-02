#!/bin/bash

make clean
make
mkdir -p ./outimages
./edgedetection imgfilenames
