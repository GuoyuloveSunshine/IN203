#!/bin/bash
for i in $(seq 1 6)
do
    mpiexec -np 2 ./simulation_async_omp.exe $i indv_cst
done