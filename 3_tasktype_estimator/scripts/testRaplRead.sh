#!/bin/bash
cd ../..
cd uarch-configure
cd rapl-read
echo "$PWD"
source /home/allan/config.sh
echo $supw | sudo -S ./rapl-read