#!/bin/bash
source ../config.sh

echo $supw | sudo -S cat /sys/class/powercap/intel-rapl/intel-rapl:0/energy_uj >> counter.txt

