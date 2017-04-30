
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <chrono>

#include <mpi.h>

struct RandomGenerator {
	int maxValue;
	RandomGenerator(int max) :
		maxValue(max) {
		}

	int operator()() {
		return rand() % maxValue;
	}
};


// Basic code to sum 2 arrays. Could've used like an MPI_Reduce,
// but I've used that before but not Scatter/Gather, and I like variety.

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cerr << "Usage: ./VecAdd n\n";
		return 1;
	}

	int len = atoi(argv[1]);
	int *a;
	int *b;
	int *c;

	srand(time(NULL));
	auto begin = std::chrono::high_resolution_clock::now();

	MPI_Init(NULL, NULL);

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	len = len - (len % world_size);
	int elementsPerProcess = len / world_size;

	if (world_rank == 0) {
		a = (int*)malloc(sizeof(int) * len);
		b = (int*)malloc(sizeof(int) * len);
		c = (int*)malloc(sizeof(int) * len);
		std::generate(a, a + len, RandomGenerator(200));
		std::generate(b, b + len, RandomGenerator(200));
	}
	//auto begin = std::chrono::high_resolution_clock::now();
	int *subsetOfA = (int*)malloc(sizeof(int) * elementsPerProcess);
	int *subsetOfB = (int*)malloc(sizeof(int) * elementsPerProcess);
	int *subsetOfC = (int*)malloc(sizeof(int ) * elementsPerProcess);
	MPI_Scatter(a, elementsPerProcess, MPI_INT, subsetOfA,
		elementsPerProcess, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(b, elementsPerProcess, MPI_INT, subsetOfB,
		elementsPerProcess, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < elementsPerProcess; i++) {
		subsetOfC[i] = subsetOfA[i] + subsetOfB[i];
	}

	MPI_Gather(subsetOfC, elementsPerProcess, MPI_INT, c, 
		elementsPerProcess, MPI_INT, 0, MPI_COMM_WORLD);


	auto end = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "ns" << std::endl;
	MPI_Finalize();

	return 0;

}



