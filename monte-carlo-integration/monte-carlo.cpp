
#include <iostream>
#include <cstdlib>
#include <mpi.h>


int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: ./monte-carlo [numIterations]" << std::endl;
    return -1;
  }

  int numIterations = atoi(argv[1]);
  int rank; // to store the relative rank of the running process
  int world_size;

  int x, y, z;
  int processFound;
  int totalFound;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size); 


 
  if (rank == -1) {} else {
    for (unsigned int i = 0; i < numIterations; i++) {
      // do a random number seed
      x = (double)random()/RAND_MAX;
      y = (double)random()/RAND_MAX;
      if (sqrt((x*x)+y*y) <= 1) {
        processFound++;
      }
    }
  }

  // Reduce the computed numbers back down to process rank 0
  MPI_Reduce(&processFound, &totalFound, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  if (rank == 0) {
    int pi = (double)totalFound / ((double)numIterations * 4 * world_size);
    std::cout << "Calculated Pi as " << pi << std::endl;
  }

  MPI_Finalize();

  return 0;
}





