#!/bin/bash
tasks=("dadd", "m4x4smul_SIMD", "v1x4smul_SIMD")

declare -A lastLine

lastLine[dadd]=1085
lastLine[m4x4smul_SIMD]=137
lastLine[v1x4smul_SIMD]=202


#for key in "${!lastLine[@]}"; do
 # echo "$key"
#done

for str in ${tasks[@]}; do
  echo $str
done

#for val in "${lastLine[@]}"; do
 # echo "$val"
#done

