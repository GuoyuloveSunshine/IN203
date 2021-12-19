# TP3 de MA Ziqi

`pandoc -s --toc tp2.md --css=./github-pandoc.css -o tp2.html`



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
Vulnerability Itlb multihit:     KVM: Mitigation: VMX disabled
```

*Des infos utiles s'y trouvent : nb core, taille de cache*

L1i cache instruction, Lid cache de donnee, L2 cache de donnee, L3 cache de donnne



## Produit scalaire 

*Expliquer les paramètres, les fichiers, l'optimisation de compil, NbSamples, ...*

on met $NbSamples=1024$,  $N=100023$, 

Nb operations (de produit scalaire) = $2N$ 

Nb acces au memoiry = $2N$  -> memory bound

NUM    | OMP      | Thread 
-----------|--------------|----------
séquentiel | 0.798682s |
1          | 0.803217s |0.697207s
2          | 0.795107s |1.30956s
3          | 0.798842s |1.20469s
4          | 0.793602s |1.16918s
8          | 0.795492s |1.26754s


*Discuter sur ce qu'on observe, la logique qui s'y cache.*

On ne constate aucune accélération, les performances sont prèsque les mêmes.  parce que il est peut-être memory-bound.. Donc le paralléliser ne l'accélère pas.

## Produit matrice-matrice

Nb operation = $2N^3$, Nb acces au memory = $3N^2$ -> CPU bound.

sizeof(double) =  64 bit = 8 octets, size(A) = $8\times1024^2= 8MB$, taille A, B et C est 24 MB.

les matrix ne rentrent pas dans cache.

N    | Temps(s) 
-----------|--------------
1023 | 22.7716 
1024          | 44.805 
1025          | 23.2527 

il y a un decalage sur N = 1024.

### Permutation des boucles

*Expliquer comment est compilé le code (ligne de make ou de gcc) : on aura besoin de savoir l'optim, les paramètres, etc. Par exemple :*

`make TestProduct.exe && ./TestProduct.exe 1024`


  ordre           | time(s) | MFlops  | MFlops(n=2048) 
------------------|---------|---------|----------------
i,j,k (origine)   | 44.0148 | 48.79 |38.439
j,i,k             | 52.0658 | 41.2435 |35.0568
i,k,j             | 73.5232 | 29.2082 |26.4976
k,i,j             | 77.5253 | 27.7004 |26.7614
j,k,i             | 22.5137 | 95.3858 |95.487
k,j,i             | 22.536 | 95.2914 |95.3726

*Discussion des résultats*

On a vu que les deux meilleurs permutations sont celles qui mettent i dans à l'arrière, d'après le cours, cette permutation exploite au mieux la mémoire cache. Les MFlops sont une mesure courante de la vitesse des microprocesseurs utilisés pour effectuer des calculs à virgule flottante. On remarque que les performance baissent quand la taille des matrices augmente.

### OMP sur la meilleure boucle 

On tient le boucle j,k,i et on fait les mesures.

​    `$ for i in $(seq 1 8); do elap=$(OMP_NUM_THREADS=$i ./TestProductMatrix.exe 1024|grep "Temps CPU"|cut -d " " -f 7); echo -e "$i\t$elap"; done > timers.out`

  OMP_NUM         | MFlops  | MFlops(n=2048) | MFlops(n=512)  
------------------|----------------|----------------|---------------
1                 | 92.2907 |92.9305|91.5818
2                 | 184.2812 | 163.8081       |179.8623
3                 | 202.5794 |155.0740|188.1605
4                 | 223.0392 | 176.3355       |184.6618
5                 | 210.1092 | 188.8248       |186.3165
6                 | 201.3335 |187.8265|197.1485
7                 | 204.6138 | 186.1237       |211.8737
8                 | 204.4405 | 191.0571       | 210.4072      

On remarque que les performances de les `OMP` saturent quand `OMP_NUM` atteint 4.


### Produit par blocs

`make TestProduct_Bloc.exe && ./TestProduct_Bloc.exe 1024`

  szBlock         | MFlops  | MFlops(n=2048) | MFlops(n=512)  
------------------|---------|----------------|----------------
origine (=max)    | 2317.28 |2223.02|2261.26
32                | 2501.2 |2402.06|2317.52
64                | 2502.29 |2554.4|2620.84
128               | 2643.07 |2640.23|1830.25
256               | 2742.71 |2832.38|2537.9
512               | 2429.06 |2465.79|2261.26
1024              | 2317.28 |2277.49|--

D'après tous les mesures, on peut trouver que la meilleure performance du séquentiel est 2700+-, quand la taille de bloc est 256.

une taille de blocs de 256 semble le meilleur $256\times256\times8\times3 = 1.5MB$

$\sqrt{3\times1024\times1024/(3\times8)} = 362$ (szBlock)


### Bloc + OMP

on choisit la taille de bloc est 256, la permutation de boucle est j,k,i.

1. `make TestProduct_Bloc.exe`
2. ``$ for i in $(seq 1 8); do elap=$(OMP_NUM_THREADS=$i ./TestProductMatrix_Bloc.exe 1024|grep "Temps CPU"|cut -d " " -f 7); echo -e "$i\t$elap"; done > timers.out`

OMP_NUM | MFlops  | MFlops(n=2048) | MFlops(n=4096) 
---------------|---------|---------|------------------------------------------------
1 | 2715.4945 | 2620.6482 | 2457.8968 
2 | 5187.9982 | 5148.3748 | 3706.5122 
3 | 4157.0771 | 4828.0030 | 3860.9267 
4 | 5397.9525 | 4797.8410 | 3596.7203 
5 | 5627.6514 | 4663.8549 | 3675.2609 
6 | 5237.8033 | 4199.7280 | 3788.6289 
7 | 4969.1405 | 3739.1192 | 3916.3873 
8 | 5108.0820 | 4208.1325 | 3855.7276 

On peut voir que la performance améliore beaucoup quand on augmente le nombre de thread, et puis, la performance sature à thread de 4.

## Tri Bitonique

Le temps pour calculer:

|                     | sur les entiers | sur les vecteurs |
| ------------------- | --------------- | ---------------- |
| **sans thread**     | 0.82            | 30.10            |
| **avec thread** = 2 | 0.38            | 19.60            |

Il y a une petite accélération  pour les tris sur les entiers et les vecteurs, par contre, il y a une grande difference entre les tris sur les entiers et les vecteurs, car la dimention de vecteur est plus grande que la dimention d'entier, il prend plus de temps pour calculer.



## Ensemble de Bhudda

```
$ make bhudda_omp.exe bhudda.exe

$ for i in $(seq 1 4); do elap=$(OMP_NUM_THREADS=$i ./bhudda_omp.exe|grep "Temps calcul"); echo -e "$i\t$elap"; done > bhudda_omp.out
```

On utilise OpenMP pour paralléliser le code Bhudda et on mesure l'accélération obtenue en fonction du nombre de threads, les résultats sont dans le tableau, comme ce programme utilise un gérénateur aléatoire, les versions séquentielles et multithread donnent des résultats très légèrement différents

| OMP_NUM    | Bhudda 1(s) | Bhudda 2(s) | Bhudda 3(s) |
| ---------- | ----------- | ----------- | ----------- |
| sequentiel | 5.489       | 5.333       | 0.332       |
| 1          | 4.108       | 3.719       | 0.234       |
| 2          | 2.563       | 1.919       | 0.122       |
| 3          | 1.932       | 1.596       | 0.095       |
| 4          | 1.862       | 1.137       | 0.075       |
| 5          | 1.780       | 1.099       | 0.074       |
| 6          | 1.656       | 1.131       | 0.081       |
| 7          | 1.689       | 1.156       | 0.074       |
| 8          | 1.832       | 1.162       | 0.111       |

# Tips 

```
	env 
	OMP_NUM_THREADS=4 ./dot_product.exe
```

```
    $ for i in $(seq 1 4); do elap=$(OMP_NUM_THREADS=$i ./TestProductOmp.exe|grep "Temps CPU"|cut -d " " -f 7); echo -e "$i\t$elap"; done > timers.out
```
