#!/bin/bash
tasks=(loadimage greyscale checkcontrast sharpencontrast copyimage sobelh sobelv combineimgs writeimage)
stops=(206       167       57            86              85        146    116    189         226)

mkdir -p gdb
mkdir -p seq

echo 'Tasks werden nun nacheinander ausgeführt'
for ((i = 0; i <${#tasks[@]}; i++)); do
  	
	taskname=${tasks[i]}
	taskstop=${stops[i]}
	gdbname=$taskname".gdb"

	echo "Task mit Index " $i "heißt " $taskname " endet in Zeile " $taskstop " wird ausgeführt und die Instruktionen werden geloggt!"
	
        #GDB-Datei vorbereiten
	echo "b "$taskname > $gdbname
	echo "r" >> $gdbname
	echo "source ci.py" >> $gdbname
	echo "b " $taskstop  >> $gdbname
	echo "ci" >> $gdbname
	echo "exit" >> $gdbname

	gdb --batch --command=$gdbname --args apptasks $taskname > $taskname.gdb.log
	mv instructionseq.txt seq/$taskname.seq
	mv $taskname".gdb" gdb/$taskname".gdb"
	mv $taskname.gdb.log gdb/$taskname.gdb.log
done
cp -f seq/*.seq ../3_tasktype_estimator/taskmapper/appseq


