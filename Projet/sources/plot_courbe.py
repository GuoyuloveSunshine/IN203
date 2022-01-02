#! /usr/bin/env python3

import sys
import numpy as np
import matplotlib.pyplot as plt
from time import sleep

def plot_courbe():
    jours, grippe, patho = np.loadtxt('Courbe_origin.dat').T
    jours2,grippe2,patho2 = np.loadtxt('Courbe_omp.dat').T
    plt.figure(figsize = (12,4))
    plt.subplot(121)
    plt.plot(jours, grippe)
    plt.plot(jours, patho)
    plt.xlabel('Duree (jours)')
    plt.ylabel('Nb cas')
    plt.title("Courbe origin")

    plt.subplot(122)
    plt.plot(jours2, grippe2)
    plt.plot(jours2, patho2)
    plt.xlabel('Duree (jours)')
    plt.ylabel('Nb cas')
    plt.title("Courbe omp")
    plt.savefig('Comparison_omp.png')
    plt.draw()

def plot_courbe2():
    jours, grippe, patho = np.loadtxt('Courbe_origin.dat').T
    jours2,grippe2,patho2 = np.loadtxt('Courbe_mpi.dat').T
    plt.figure(figsize = (12,4))
    plt.subplot(121)
    plt.plot(jours, grippe)
    plt.plot(jours, patho)
    plt.xlabel('Duree (jours)')
    plt.ylabel('Nb cas')
    plt.title("Courbe origin")

    plt.subplot(122)
    plt.plot(jours2, grippe2)
    plt.plot(jours2, patho2)
    plt.xlabel('Duree (jours)')
    plt.ylabel('Nb cas')
    plt.title("Courbe mpi")
    plt.savefig('Comparison_mpi.png')
    plt.draw()

if __name__ == '__main__':

    if '-h' in sys.argv:
        print(f'Usage: {sys.argv[0]} [-r|--refresh]')
        exit(0)

    if '-r' in sys.argv or '--refresh' in sys.argv:
        while True:
            plot_courbe()
            plt.pause(0.2)
            plt.clf()
            
    else:
        plot_courbe()
        plot_courbe2()
        plt.show()
