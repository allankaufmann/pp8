#!/bin/bash
# Erzeuge einen Dateinamen mit dem heutigen Datum
dateiname="logs/output/check_preconditions_$(date +'%Y-%m-%d_%H-%M-%S').log"
touch "$dateiname"
./main 2>&1 I | tee "$dateiname"