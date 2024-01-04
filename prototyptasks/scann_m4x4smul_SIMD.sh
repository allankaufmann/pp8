gcc -g task_m4x4smul_SIMD.c -o task_m4x4smul_SIMD
gdb --batch --command=task_m4x4smul_SIMD.gdb --args task_task_m4x4smul_SIMD
mv instructionseq.txt task_m4x4smul_SIMD.seq