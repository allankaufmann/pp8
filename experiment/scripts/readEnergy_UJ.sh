#!/bin/bash
source /home/allan/config.sh
mkdir -p logs
echo $supw | sudo -S cat /sys/class/powercap/intel-rapl/intel-rapl:0/energy_uj > logs/counter.txt
echo ""

