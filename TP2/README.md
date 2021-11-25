# TP2 de MA Ziqi

`pandoc -s --toc tp2.md --css=./github-pandoc.css -o tp2.html`



## Questions du cours

1.  Il y a un interblocage quand 1 exécute avant 0, après 2 reçoit les données vient de 1 et 2 passe à l'état `MPI_Ssend` où 0 est aussi dans l'état `MPI_Ssend`.
2. Il n'y a pas d'interblocage quand 0 exécute avant 1,  après 0 envoie les donnés dans à 2, il passe à `MPI_Recv` et 2 reçoit les donnés de 0, il passe à `MPI_Ssend` et il envoie les donnée à 0, après il passe à l'état `MPI_Recv`......
3. Pour éviter l'interblocage, il faut indiquer où vient les donnés.

## Mandelbrot 

*Expliquer votre stratégie pour faire une partition équitable des lignes de l'image entre chaque processus*

           | Taille image : 800 x 600 | 
-----------+---------------------------
séquentiel |
1          | On divise `H` par les `nbp` serveurs.
2          | dans chaque serveurs, il calcule `H/nbp` de ligne
3          | après, le serveurs 0 collecte les résultats en utilisant `MPI_Gather`
4          |
8          |              

*Discuter sur ce qu'on observe, la logique qui s'y cache.*

il gagne du temps.

*Expliquer votre stratégie pour faire une partition dynamique des lignes de l'image entre chaque processus*

           | Taille image : 800 x 600 | 
-----------+---------------------------
séquentiel |
1          | Le maitre 0 envoye les lignes pour tous les processeur.
2          | Après il passe à l'état de Recv, quand un processeur finit son tache, il l'envoie à 0 et 0 lui renvoie un autre ligne. 
3          | Quand tous les lignes sont calculés, 0 envoie -1 aux processeur.
4          | le processeur reçoit le ligne et le calcule et l'envoie à 0, jusqu'à il reçoit le demande -1
8          |          



## Produit matrice-vecteur



*Expliquer la façon dont vous avez calculé la dimension locale sur chaque processus, en particulier quand le nombre de processus ne divise pas la dimension de la matrice.*

on supposera que `N` est divisible par le nombre de tâches `nbp` exécutées.

Quand on divise le matrice par colonne, on initialise le matrice par `dim` de ligne et `dim/nbp` de colonne, et après pour chaque `rank`, on remplit les lignes et les colonnes par:
$$
A_{rank}(i,j) = (i+j+rank + dim/nbp)\%dim
$$
après on calcule chaque $A_{rank} *U[rank]$, on les récupère par `MPI_Allreduce`.



Quand on divise le matrice par ligne, on initialise le matrice par `dim/nbp` de ligne et `dim` de colonne, et après pour chaque `rank`, on remplit les lignes et les colonnes par:
$$
A_{rank}(i,j) = (i+j+rank + dim/nbp)\%dim
$$
après on calcule chaque $A_{rank} *U$, on les récupère par `MPI_Allgather`.
