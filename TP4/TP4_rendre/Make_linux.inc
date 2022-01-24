CC = mpicc
CXX = mpic++ 
LIB = -lmpi 
MPIOPT = -oversubscribe

CC2 = gcc -fopenmp
CXX2 = g++ -fopenmp
LIB2 = -lm -ltbb -lpthread

