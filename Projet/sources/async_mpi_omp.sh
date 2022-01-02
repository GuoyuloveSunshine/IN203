#!/bin/bash
for proc in $(seq 2 3)
do
    for thread in $(seq 1 4)
    do
        mpiexec -np $proc ./simulation_async_mpi_omp.exe $proc $thread
    done
done