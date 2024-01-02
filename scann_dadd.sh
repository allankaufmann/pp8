cd prototyptasks
gcc -g task_dadd.c -o task_dadd
gdb --batch --command=dadd.gdb --args task_dadd
mv instructionseq.txt dadd.seq
cd ..
