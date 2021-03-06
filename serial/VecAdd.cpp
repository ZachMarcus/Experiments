
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <chrono>

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
	srand(time(NULL));

	int* a = (int*)malloc(sizeof(int) * len);
	int* b = (int*)malloc(sizeof(int) * len);
	int* c = (int*)malloc(sizeof(int) * len);
	auto begin = std::chrono::high_resolution_clock::now();

	std::generate(a, a + len, RandomGenerator(200));
	std::generate(b, b + len, RandomGenerator(200));

	for (int i = 0; i < len; i++) {
		c[i] = a[i] + b[i];
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "ns" << std::endl;


	return 0;

}



