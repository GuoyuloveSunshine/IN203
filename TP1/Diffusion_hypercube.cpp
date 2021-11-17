# include <cstdlib>
# include <sstream>
# include <string>
# include <fstream>
# include <iostream>
# include <iomanip>
# include <math.h>
# include <mpi.h>


int main( int nargs, char* argv[] ){
	MPI_Init( &nargs, &argv );
	MPI_Comm globComm;
	MPI_Comm_dup(MPI_COMM_WORLD, &globComm);
	int nbp;
	MPI_Comm_size(globComm, &nbp);
	int rank;
	MPI_Comm_rank(globComm, &rank);

  	int dim = log2(nbp);
	int jeton = 666;
	MPI_Status DIFFU;
	if(rank == 0){
		for(int i = 0;i<dim;i++){
			MPI_Send(&jeton,1,MPI_INT,std::pow(2,i),345,globComm);	
		}
		std::cout << "Je suis le processus "<<rank<<" et le jeton est " << jeton << std::endl;
	}
	else if (rank >= std::pow(2, dim)) {
    	MPI_Finalize();
    	return EXIT_SUCCESS;
  	}
	else {
		int j = log2(rank);

		MPI_Recv(&jeton, 1, MPI_INT, rank - std::pow(2 ,j), 345, globComm, &DIFFU);
		for (int i = j + 1; i < dim; i++) {

			MPI_Send(&jeton, 1, MPI_INT, std::pow(2, i) + rank, 345, globComm);

		}
		std::cout << "Je suis le processus " << rank << " et le jeton est " << jeton << std::endl;
	}
	MPI_Finalize();
	return EXIT_SUCCESS;
}
