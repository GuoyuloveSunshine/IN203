#!/bin/bash
for i in $(seq 1 6)
do
    ./fourier_compression_OMP.exe data/tiny_lena_gray.png 0.8 $i
done