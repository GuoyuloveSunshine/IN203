# include <iostream>
# include <cstdlib>
# include <mpi.h>

int main( int nargs, char* argv[] )
{
	// On initialise le contexte MPI qui va s'occuper :
	//    1. Créer un communicateur global, COMM_WORLD qui permet de gérer
	//       et assurer la cohésion de l'ensemble des processus créés par MPI;
	//    2. d'attribuer à chaque processus un identifiant ( entier ) unique pour
	//       le communicateur COMM_WORLD
	//    3. etc...

	MPI_Init( &nargs, &argv );
	// Pour des raison préfère toujours cloner le communicateur global
	// MPI_COMM_WORLD qui gère l'ensemble des processus lancés par MPI.
	MPI_Comm globComm;
	MPI_Comm_dup(MPI_COMM_WORLD, &globComm);
	// On interroge le communicateur global pour connaître le nombre de processus
	// qui ont été lancés par l'utilisateur :
	int nbp;
	MPI_Comm_size(globComm, &nbp);
	// On interroge le communicateur global pour connaître l'identifiant qui
	// m'a été attribué ( en tant que processus ). Cet identifiant est compris
	// entre 0 et nbp-1 ( nbp étant le nombre de processus qui ont été lancés par
	// l'utilisateur )
	int rank;
	MPI_Comm_rank(globComm, &rank);

	int token;
	MPI_Status bordereau;
	if(rank ==0 ){
		//je suis le rank 0
		token = 1234;
		std::cout << "Jeton initial: " << token <<std::endl;
		MPI_Send(&token,1,MPI_INT,1,4242,globComm);
		MPI_Recv(&token,1,MPI_INT,nbp-1,4242,globComm,&bordereau);
	}
	else{
		//je suis les autres rank.
		MPI_Recv(&token,1,MPI_INT,rank-1,4242,globComm,&bordereau);
		token +=1;
		std::cout << "Jeton processus: " << rank<<" : "<< token <<std::endl;
		int to = (rank+1) % nbp;
		MPI_Send(&token,1,MPI_INT,to,4242,globComm);
	}
	// A la fin du programme, on doit synchroniser une dernière fois tous les processus
	// afin qu'aucun processus ne se termine pendant que d'autres processus continue à
	// tourner. Si on oublie cet instruction, on aura une plantage assuré des processus
	// qui ne seront pas encore terminés.
	MPI_Finalize();
	return EXIT_SUCCESS;
}
