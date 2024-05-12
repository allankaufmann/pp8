git -C epEBench pull || git clone https://github.com/RobertMueller/epEBench.git
cd epEBench
make
cd ..
git -C GDBInstructionScanner pull || git clone https://gitlab.abo.fi/sholmbac/GDBInstructionScanner.git
#cp GDBInstructionScanner/ci.py tests/ci.py
#cp GDBInstructionScanner/ci.py sampleapp/ci.py
#cp GDBInstructionScanner/test.c tests/test.c
#git -C uarch-configure pull || git clone https://github.com/deater/uarch-configure.git


#cpupower
#uname -r Prüfen des Kernels, dann folgende Version installieren
sudo apt install linux-tools-6.5.0-27-generic
cd edgedetection
make
./getfiles.sh
sudo apt install python3-pil
sudo apt install python3-numpy