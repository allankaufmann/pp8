#!/bin/bash
tasks=('dadd' 'm4x4smul_SIMD' 'v1x4smul_SIMD')
stops=(1085,   137,            202)

echo 'Tasks werden nun nacheinander ausgeführt'
for ((i = 0; i <${#tasks[@]}; i++)); do
  	echo "Task mit Index " $i ":" ${tasks[i]}
	echo ${tasks[i]} ${stops[i]}
        cd ..
	cd build
	./prototyptaskscpp ${tasks[i]}
	cd ..
	cd tasks

done



