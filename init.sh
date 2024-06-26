#make installieren
sudo apt-get update
sudo apt-get -y install make

# epEBench auschecken und bauen
git -C epEBench pull || git clone https://github.com/RobertMueller/epEBench.git
cd epEBench

# Im Benchmark epEBench lässt sich der Task-Typ m4x4smul nicht mehr ausführen, da ein cach-Fehler provoziert wird, der zum Abbruch führt. 
# Dies wurde auskommentiert, da dieser Fehler für das Experiment nicht relevant ist.
sed -i '/idx = (idx + 16) % MEM_SIZE-20;/c\idx = (idx + 16) % MEM_SIZE/*-20*/;' ebloads.cpp

# In der Konfigurationsdatei von epEBench fehlt das Tasktyp ssub32_SIMD als einzelnes Modell. Dieses wird hier eingefügt.
echo "model=ssub32_SIMD" >> ebmodels.ini
echo " ssub32_SIMD=1" >> ebmodels.ini
echo "end_model" >> ebmodels.ini
make
cd ..

## Die Quelldatei für den GDBInstructionScanner heißt ci.py und wurde bereits mit den Artefakten eingecheckt. Die Quelldatei muss daher nicht mehr ausgecheckt werden.
#git -C GDBInstructionScanner pull || git clone https://gitlab.abo.fi/sholmbac/GDBInstructionScanner.git

#cp GDBInstructionScanner/ci.py sampleapp/ci.py
#git -C uarch-configure pull || git clone https://github.com/deater/uarch-configure.git


#Installation von cpupower
#uname -r Prüfen des Kernels, dann folgende Version installieren
sudo apt install linux-tools-6.5.0-27-generic

#edgedetection bauen und vorbereiten
cd edgedetection
make
./getfiles.sh
sudo apt install python3-pil
sudo apt install python3-numpy
cd ..

#cmake installieren
sudo apt-get install cmake

#Artefakte bauen
cd 1_tasktype_analyzer
make
cd ..

cd 2_apptask_analyzer
make
cd ..

cd 3_tasktype_estimator
./run_build.sh


