git clone https://github.com/RobertMueller/epEBench.git
cd epEBench
make
cd ..
git clone https://gitlab.abo.fi/sholmbac/GDBInstructionScanner.git
cp GDBInstructionScanner/ci.py tests/ci.py