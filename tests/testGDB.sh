cd GDBInstructionScanner
gcc -g test.c -o myprog
gdb --batch --command=test.gdb --args myprog
mv instructionseq.txt test.seq
cd ..
