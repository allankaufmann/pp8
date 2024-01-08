gcc -g main.c -o sampleapp
gdb --batch --command=sampleapp.gdb --args sampleapp > sampleapp.gdb.log
mv instructionseq.txt sampleapp.seq
