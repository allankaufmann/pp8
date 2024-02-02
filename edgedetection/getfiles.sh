#!/bin/bash

mkdir -p ./imgzips
for fname in $(cat ./filenames)
do
    wget https://vision.middlebury.edu/stereo/data/scenes2014/zip/$fname
    mv $fname ./imgzips
done
