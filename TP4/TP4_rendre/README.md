## OpenMP et MPI

#### Avec MPI

Les programmes qu'on écrit est 'Mandelbrot_mpi.cpp', ensuite, dans le console, on tape: 

```
make all

mpiexec -np 6 ./Mandelbrot_mpi.exe 
```

On met 6 lignes pour comparer.

Le temps calcul ensemble mandelbrot est : 8.98488s.

#### Avec OpenMP

Après on écrit le programme 'Mandelbrot_openmp.cpp', ensuite, dans le console, on tape en faisant varier le nombre de threads:

```
make all

./Mandelbrot_openmp.exe
```



| nombre de threads | Temps calcul (s) |
| ----------------- | ---------------- |
| 2                 | 20.4182          |
| 3                 | 21.1685          |
| 4                 | 21.8232          |
| 5                 | 29.6504          |
| 6                 | 32.7163          |
| 7                 | 31.6782          |
| 8                 | 42.3324          |

Je ne sais pas pourquoi plus de threads on met, plus de temps pour le calcul.

#### Maitre_esclave

Ensuite, on exécute le programme 'Maitre_esclave.cpp', et on mesure le temps de calcul pour le thread est 6, dans le console, on tape:

```
make all

mpiexec -np 6 ./Maitre_esclave
```

Le temps calcul ensemble mandelbrot est : 7. 04588s

#### Sequentiel

Enfin, on exécute le programme 'Mandelbrot_seq.cpp', dans le console, on tape:

```
make all

./Mandelbrot_seq.exe
```

Le temps calcul ensemble mandelbrot est : 20.3421s.

#### Conclusion

En conclusion, le temps de calcul pour Mandelbrot est **Maitre_esclave < MPI < Sequentiel < OpenMP**.





## Parallélisation avec TBB

les algorithmes originaux et les algorithmes de TBB sont dans le document 'raytracer.cpp' et 'raytracer_tbb.cpp', après:

```
make all

./raytracer.exe

./raytracer_tbb.exe
```

Le temps calcul sans accélération est: 4.29409s.

Le temps calcul avec le TBB est: 1.86978s.

Après on mesure le temps pour les partitionner différents et les tailles de grain différentes

|                    | sz_grain = 2 | sz_grain = 16 | sz_grain = 32 | sz_grain = 64 |
| ------------------ | ------------ | ------------- | ------------- | ------------- |
| simple_partitioner | 1.8531s      | 1.8526s       | 1.9315s       | 2.0111s       |
| auto_partitioner   | 1.8740s      | 1.8836s       | 1.9508s       | 1.9461s       |
| static_partitioner | 2.1135s      | 2.0160s       | 2.0200s       | 1.9508s       |

