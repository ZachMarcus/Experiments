
#include <iostream>

#include <mpi.h>





int main(int argc, char** argv) {

  int rank; // to store the relative rank of the running process

  int x, y, z;
  int processFound;
  int totalFound;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
  if (rank == 0) {
    continue;
  } else {
    for (unsigned int i = 0; i < 100; i++) {
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
    int pi = (double)totalFound / ((double)processComputed*4*numProcess);
    std::cout << "Calculated Pi as " << pi << std::endl;
  }

  return 0;
}





