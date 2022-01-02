#!/bin/bash
for i in $(seq 2 5)
do
    mpiexec -np $i ./simulation_async_mpi.exe $i
done