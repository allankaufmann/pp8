#!/bin/bash
# Erzeuge einen Dateinamen mit dem heutigen Datum
dateiname="logs/output/estimation_$(date +'%Y-%m-%d_%H-%M-%S').log"
touch "$dateiname"
./main 2>&1 Z 7| tee "$dateiname"
./main 2>&1 Z 0| tee "$dateiname"