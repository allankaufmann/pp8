git -C epEBench pull || git clone https://github.com/RobertMueller/epEBench.git
cd epEBench
make
cd ..
git -C GDBInstructionScanner pull || git clone https://gitlab.abo.fi/sholmbac/GDBInstructionScanner.git
cp GDBInstructionScanner/ci.py tests/ci.py
cp GDBInstructionScanner/ci.py prototyptaskscpp/ci.py
cp GDBInstructionScanner/test.c tests/test.c
