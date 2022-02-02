# Rapport

Ziqi MA

## lscpu

```
Architecture:                    x86_64
CPU op-mode(s):                  32-bit, 64-bit
Byte Order:                      Little Endian
Address sizes:                   39 bits physical, 48 bits virtual
CPU(s):                          4
On-line CPU(s) list:             0-3
Thread(s) per core:              2
Core(s) per socket:              2
Socket(s):                       1
NUMA node(s):                    1
Vendor ID:                       GenuineIntel
CPU family:                      6
Model:                           142
Model name:                      Intel(R) Core(TM) i5-7200U CPU @ 2.50GHz
Stepping:                        9
CPU MHz:                         2700.000
CPU max MHz:                     3100,0000
CPU min MHz:                     400,0000
BogoMIPS:                        5399.81
Virtualization:                  VT-x
L1d cache:                       64 KiB
L1i cache:                       64 KiB
L2 cache:                        512 KiB
L3 cache:                        3 MiB
NUMA node0 CPU(s):               0-3
```

Ce sont des paramètres des mon ordinateur.

## 1. Mesurez les temps

```
$ make fourier_compression.exe 
$ ./fourier_compression.exe data/tiny_lena_gray.png 0.8
$ ./fourier_compression.exe data/tiny_lena_gray.png 0.1
$ ./fourier_compression.exe data/small_lena_gray.png 0.1
$ ./fourier_compression.exe data/small_lena_gray.png 0.8
```

Le temps pour mesurer le choix de taux, pour le DFT est pour la reconstitution de l’image ”compressée” est présenté si-dessous

| Temps(s)       | tiny_0.1    | tiny_0.8    | small_0.1  | small_0.8 |
| -------------- | ----------- | ----------- | ---------- | --------- |
| taux           | 0.000339969 | 0.000947828 | 0.00707098 | 0.0108    |
| DFT            | 0.540888    | 0.544876    | 134.949    | 132.701   |
| reconstitution | 0.0744656   | 0.516936    | 17.256     | 136.378   |



 On a constaté alors, le temps pour choisir le taux est très peu, par rapport aux deux autre, et on peut le négliger dans les questions suivantes. Et le temps pour le DFT dépend de la taille de l'image, le temps pour la reconstitution dépend du Nombre de coefficients de fourier restant.

## 2. Parallélisez à l’aide d’OpenMP

On ajoute `#pragma parallel for schedule(static) num_threads(OMP_NUM_THREADS)` dans la fonction `discretTransformFourier` et la fonction `inversePartialDiscretTransformFourier`, juste avant le premier `for`, et on mesure le temps passé pour DFT et la reconstitution de l'image plus petit, donc on choisit le `tiny_lena_gray.pny` pour un taux de `0.8`, la parallélisation est fait sur les pixels. On a testé le nombre de thread de 1 à 6, on écrit un `OMP.sh`.

```bash
#!/bin/bash
for i in $(seq 1 6)
do
    ./fourier_compression_OMP.exe data/tiny_lena_gray.png 0.8 $i
done
```

Dans le console, on tape,

```
$ make fourier_compression_OMP.exe

$ bash OMP.sh
```

| Thread                  | 1           | 2           | 3          | 4           | 5           | 6           |
| ----------------------- | ----------- | ----------- | ---------- | ----------- | ----------- | ----------- |
| DFT_temps(s)            | 0.652432    | 0.307636    | 0.292313   | 0.273526    | 0.266188    | 0.254956    |
| taux_temps(s)           | 0.000943463 | 0.0011344   | 0.00102285 | 0.000979437 | 0.000998477 | 0.00113161  |
| reconstitution_temps(s) | 0.511841    | 0.262243    | 0.244766   | 0.196926    | 0.201635    | 0.208364    |
| Total_temps(s)          | 1.165216463 | 0.5710134   | 0.53810185 | 0.471431437 | 0.468821477 | 0.46445161  |
| Acceleration            | 0.912070728 | 1.861181941 | 1.97501612 | 2.254325326 | 2.266875304 | 2.288203561 |

On a vu alors, il y a des accélération en utilisant le OMP, mais la performance est le mieux, quand nombre de thread est 4, et la performance n'améliore plus si on ajoute le nombre de thread plus que 4, c'est raisonnable, car, le core de notre CPU est 4.

## 3. Première parallélisation avec MPI

```
$ make fourier_compression_MPI.exe 

$ mpiexec -np 4 ./fourier_compression_MPI.exe data/tiny_lena_gray.png 0.8
```

Le temps total est 0.73054s, on a une accélération de 1.45.