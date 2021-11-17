# include <chrono>
# include <random>
# include <cstdlib>
# include <sstream>
# include <string>
# include <fstream>
# include <iostream>
# include <iomanip>
# include <mpi.h>

// Attention , ne marche qu'en C++ 11 ou supérieur :
long dart_in_circle_counter( unsigned long nbSamples ){
    typedef std::chrono::high_resolution_clock myclock;
    myclock::time_point beginning = myclock::now();
    myclock::duration d = beginning.time_since_epoch();
    unsigned seed = d.count();
    std::default_random_engine generator(seed);
    std::uniform_real_distribution <double> distribution ( -1.0 ,1.0);
    unsigned long nbDarts = 0;
    // Throw nbSamples darts in the unit square [-1 :1] x [-1 :1]
    for ( unsigned sample = 0 ; sample < nbSamples ; ++ sample ) {
        double x = distribution(generator);
        double y = distribution(generator);
        // Test if the dart is in the unit disk
        if ( x*x+y*y<=1 ) nbDarts ++;
    }
    return nbDarts;
}

long int recevoir_bloquant(int rank, int nbp, MPI_Comm globComm){
	MPI_Status CALCULPI;
	int nb_dart_per_proc = 1000000;
	unsigned long nb_darts_cercle = 0;
	unsigned long nb_darts_temp = 0;
	unsigned long nb_darts_total = (nbp-1)*nb_dart_per_proc;

	typedef std::chrono::high_resolution_clock myclock; 
    myclock::time_point debut = myclock::now();
	// Rajout de code....
	if(rank ==0){
		// On choisit 0 comme maitre
		std::cout << "je suis [0], et j'attends les retours des autres tâches" << std::endl;
		for(int i = 1; i<nbp;i++){
			MPI_Recv(&nb_darts_temp,1,MPI_UNSIGNED_LONG,i,666,globComm,&CALCULPI);
			std::cout << "je suis [0], et je reçois: [" << i <<"], " << nb_darts_temp << std::endl;
			nb_darts_cercle += nb_darts_temp;
			nb_darts_temp = 0;
		}
		double pi = (4*(double)nb_darts_cercle/(double)nb_darts_total);
		std::cout << "pi a peu près égal à " << pi << std::endl;
		// On calcule la duration.
		myclock::duration duree = myclock::now() - debut;
        std::cout << "Durée d'exécution du programme : " << duree.count() << std::endl;
		return (long int)duree.count();
	}
	else{
		nb_darts_temp = 0;
		nb_darts_temp = dart_in_circle_counter(nb_dart_per_proc);
		std::cout << "je suis ["<<rank<<"], et j'envoie mon résultat " << nb_darts_temp << " a [0]"<< std::endl;
		MPI_Send(&nb_darts_temp,1,MPI_UNSIGNED_LONG,0,666,globComm);
	}
}

long int recevoir_non_bloquant(int rank, int nbp, MPI_Comm globComm){
	MPI_Request REQS;
	MPI_Status CALCULPI;
	int nb_dart_per_proc = 1000000;
	unsigned long nb_darts_cercle = 0;
	unsigned long nb_darts_temp = 0;
	unsigned long nb_darts_total = (nbp-1)*nb_dart_per_proc;

	typedef std::chrono::high_resolution_clock myclock; 
    myclock::time_point debut = myclock::now();
	if(rank ==0){
		// On choisit 0 comme maitre
		std::cout << "je suis [0], et j'attends les retours des autres tâches" << std::endl;
		for(int i = 1; i<nbp;i++){
			MPI_Irecv(&nb_darts_temp,1,MPI_UNSIGNED_LONG,i,666,globComm,&REQS);
			MPI_Waitall (1,&REQS ,&CALCULPI);
			std::cout << "je suis [0], et je reçois: [" << i <<"], " << nb_darts_temp << std::endl;
			nb_darts_cercle += nb_darts_temp;
			nb_darts_temp = 0;
		}
		double pi = (4*(double)nb_darts_cercle/(double)nb_darts_total);
		std::cout << "pi a peu près égal à " << pi << std::endl;
		// On calcule la duration.
		myclock::duration duree = myclock::now() - debut;
        std::cout << "Durée d'exécution du programme : " << duree.count() << std::endl;
		return (long int)duree.count();
	}
	else{
		nb_darts_temp = 0;
		nb_darts_temp = dart_in_circle_counter(nb_dart_per_proc);
		std::cout << "je suis ["<<rank<<"], et j'envoie mon résultat " << nb_darts_temp << " a [0]"<< std::endl;
		MPI_Send(&nb_darts_temp,1,MPI_UNSIGNED_LONG,0,666,globComm);
	}
	
}

int main( int nargs, char* argv[] ){
	MPI_Init( &nargs, &argv );
	MPI_Comm globComm;
	MPI_Comm_dup(MPI_COMM_WORLD, &globComm);
	int nbp;
	MPI_Comm_size(globComm, &nbp);
	int rank;
	MPI_Comm_rank(globComm, &rank);

	long int temps_pour_bloquant = recevoir_bloquant(rank,nbp,globComm);
	long int temps_pour_non_bloquant = recevoir_non_bloquant(rank,nbp,globComm);
	
	std::cout<<std::endl;
	std::cout<<"Le temps pour recevoir bloquant: "<< temps_pour_bloquant << std::endl;
	std::cout<<"Le temps pour recevoir non bloquant: "<< temps_pour_non_bloquant << std::endl;	
	// A la fin du programme, on doit synchroniser une dernière fois tous les processus
	// afin qu'aucun processus ne se termine pendant que d'autres processus continue à
	// tourner. Si on oublie cet instruction, on aura une plantage assuré des processus
	// qui ne seront pas encore terminés.
	MPI_Finalize();
	return EXIT_SUCCESS;
}
