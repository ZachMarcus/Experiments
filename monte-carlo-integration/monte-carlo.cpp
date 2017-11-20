#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <mpi.h>
#include <random>


// C++ program to approximate pi using the Monte Carlo method
// Distributes work across processes using MPI
// Makes use of mersenne twister random number generator
// Uses MPI based reduction to gather results back from processes
int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: ./monte-carlo [numIterations]" << std::endl;
    return -1;
  }

  int numIterations = atoi(argv[1]);
  int rank;       // to store the relative rank of the running process
  int world_size; // to know how many processes are running

  double x, y;
  int processFound = 0; // to store how many numbers fall in the acceptable
                        // range in this process
  int sumFound = 0;   // for the reduction
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size); 
  int numIterations = atoi(argv[1]) / world_size;
  // C++11 random number generation
  std::random_device randomDevice;
  std::mt19937 mersenneTwister(randomDevice());
  std::uniform_real_distribution<> numberDistribution(0,1); 
 
  for (int i = 0; i < numIterations; i++) {
    // do a random number seed
    x = numberDistribution(mersenneTwister);
    y = numberDistribution(mersenneTwister);
    if (sqrt((x*x)+(y*y)) <= 1) { // it falls within the montecarlo square
      processFound++;
    }
  }

  // Reduce the computed numbers back down to process rank 0
  MPI_Reduce(&processFound, &sumFound, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  if (rank == 0) {
    double pi = (4 * (double)sumFound) / 
               ((double)numIterations * (double)world_size); 
    std::cout << std::setprecision(8) << "Calculated Pi as " << pi << "\n";
  }

  MPI_Finalize();

  return 0;
}





