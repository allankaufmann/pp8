#!/bin/bash
tasks=('dadd' 'm4x4smul_SIMD' 'v1x4smul_SIMD')
stops=(1085    137            202)

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



