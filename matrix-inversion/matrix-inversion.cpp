//USE DPOTRI FROM FORTRAN 
//OR CHOL2inv
//
//
//
//
//


#include <iostream>
#include <cstdlib>
#include <random>
#include <omp.h>

const int MATRIX_SIZE = 5;


//int determinant(int matrix[][MATRIX_SIZE]);
//void cofactor(int matrix[][MATRIX_SIZE]);
//void transpose(int matrix[][MATRIX_SIZE]);
//https://en.wikipedia.org/wiki/Gaussian_elimination






void initMatrix(int matrix[][MATRIX_SIZE]) {
  #pragma omp parallel for
  for (int i = 0; i < MATRIX_SIZE; i++) {
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(0, 1000);
    for (int j = 0; j < MATRIX_SIZE; j++) {
      matrix[i][j] = distribution(generator);
    }
  }
  return;
}


template<class T>
void printMatrix(T matrix[][MATRIX_SIZE]) {
  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      std::cout << matrix[i][j] << " ";
    }
    std::cout << "\n";
  }
  return;
}

void invertMatrixSerial(int matrix[][MATRIX_SIZE]) {

}


int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Args wrong. Usage: ./matrix-inversion [shouldPrint]" << std::endl;
    exit(1);
  }

  int shouldPrint = atoi(argv[1]);

  int matrix[MATRIX_SIZE][MATRIX_SIZE];
  initMatrix(matrix);
  if (shouldPrint) {
    printMatrix(matrix);
  }
  invertMatrixSerial(matrix);
  if (shouldPrint) {
    printMatrix(matrix);
  }
  return 0;
}



