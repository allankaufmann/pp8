#!/bin/bash
cd ..
cd uarch-configure
cd rapl-read
echo "$PWD"
source ../config.sh
echo $supw | sudo -S ./rapl-read