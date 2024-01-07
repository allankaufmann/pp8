#!/bin/bash
tasks=('dadd' 'm4x4smul_SIMD' 'v1x4smul_SIMD' 'dmul64_SIMD' 'smul32_SIMD' 'ssub32_SIMD' 'dsub64_SIMD', 'dmem64_SIMD' 'smem32_SIMD' 'vmov_SIMD' 'vconvert_SIMD' 'm4x4smul' 'dmul' 'imul' 'iadd' 'icompare' 'logic' 'branch' 'imem' 'dmem' 'imov' 'shift' 'bitbyte' 'nop')
stops=(1085    137            202              234          266           300           334            360            387           420        453              885       1054    1116   1147   1214       1282   1349     1377   1406    1476   1549    1620      1657)

gcc -g main.cpp -o main

echo 'Tasks werden nun nacheinander ausgeführt'
for ((i = 0; i <${#tasks[@]}; i++)); do
  	
	taskname=${tasks[i]}
	taskstop=${stops[i]}
	gdbname=$taskname".gdb"

	echo "Task mit Index " $i "heißt " $taskname " und endet in Zeile " $taskstop
	
        #GDB-Datei vorbereiten
	echo "b run_"$taskname > $gdbname
	echo "r" >> $gdbname
	echo "source ci.py" >> $gdbname
	echo "b " $taskstop  >> $gdbname
	echo "ci" >> $gdbname
	echo "exit" >> $gdbname

	gdb --batch --command=$gdbname --args main $taskname
	mv instructionseq.txt $taskname.seq

	#cd build
	#./prototyptaskscpp $taskname
	#cd ..

done



