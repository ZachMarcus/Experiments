
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


int main(int argc, char** argv) {
	if (argc != 2) {
		std::cerr << "Usage: ./VecAdd n\n";
		return 1;
	}

	int len = atoi(argv[1]);

	int a[len];
	int b[len];
	int c[len];
	srand(time(NULL));

	std::generate(a, a + len, RandomGenerator(200));
	std::generate(b, b + len, RandomGenerator(200));
	auto begin = std::chrono::high_resolution_clock::now();


	MPI_Init(NULL, NULL);

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);

	std::cout << "Hello from processor " << processor_name << ", rank " << world_rank << " out of " << world_size << " processors\n";
	MPI_Finalize();
	for (int i = 0; i < len; i++) {
		c[i] = a[i] + b[i];
	}


	auto end = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "ns" << std::endl;

	// for (int i = 0; i < c.size(); i++) {
	// 	std::cout << a[i] << " + " << b[i] << " = " << c[i] << std::endl;
	// }

	return 0;

}



