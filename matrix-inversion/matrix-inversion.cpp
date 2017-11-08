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

const int MATRIX_SIZE = 3;


//int determinant(int matrix[][MATRIX_SIZE]);
//void cofactor(int matrix[][MATRIX_SIZE]);
//void transpose(int matrix[][MATRIX_SIZE]);
//https://en.wikipedia.org/wiki/Gaussian_elimination

template<class T>
T calculateDeterminant(T array[][MATRIX_SIZE], int size);
template<class T>
void calculateCofactor(T array[MATRIX_SIZE][MATRIX_SIZE], T factor);
template<class T>
void calculateTranspose(T array[MATRIX_SIZE][MATRIX_SIZE], T factors[MATRIX_SIZE][MATRIX_SIZE], float r);


template<class T>
T calculateDeterminant(T array[][MATRIX_SIZE], int size) {
  T temp = 1;
  T determinant = 0;
  T tempArray[MATRIX_SIZE][MATRIX_SIZE];
  if (size == 1) {
    return array[0][0];
  } else {
    for (int c = 0; c < size; c++) {
      int m = 0;
      int n = 0;
      for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
          tempArray[i][j] = 0;
          if (i != 0 && j!= c) {
            tempArray[m][n] = array[i][j];
            if (n < (size - 2)) {
              n++;
            } else {
              n = 0;
              m++;
            }
          }
        }
      }
      determinant = determinant + temp * (array[0][c] * calculateDeterminant(tempArray, size-1));
      temp = -1 * temp;
    }
  }
  return determinant;
}


template<class T>
void calculateCofactor(T array[MATRIX_SIZE][MATRIX_SIZE], T factor) {
  T tempArray[MATRIX_SIZE][MATRIX_SIZE], factors[MATRIX_SIZE][MATRIX_SIZE];
  for (int q = 0; q < factor; q++) {
    for (int p = 0; p < factor; p++) {
      int m = 0;
      int n = 0;
      for (int i = 0; i < factor; i++) {
        for (int j = 0; j < factor; j++) {
          if (i != q && j != p) {
            tempArray[m][n] = array[i][j];
            if (n < (factor - 2)) {
              n++;
            } else {
              n = 0;
              m++;
            }
          }
        }
      }
      factors[q][p] = pow(-1, q + p) * calculateDeterminant(tempArray, factor - 1);
    }
  }
  calculateTranspose(array, factors, factor);
}


template<class T>
void calculateTranspose(T array[MATRIX_SIZE][MATRIX_SIZE], T factors[MATRIX_SIZE][MATRIX_SIZE], float r) {
  T tempArray[MATRIX_SIZE][MATRIX_SIZE];
  T inverseArray[MATRIX_SIZE][MATRIX_SIZE];
  T d;
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < r; j++) {
      tempArray[i][j] = factors[j][i];
    }
  }
  d = calculateDeterminant(array, r);
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < r; j++) {
      array[i][j] = tempArray[i][j] / d;
    }
  }
}



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

template<class T>
void invertMatrixSerial(T matrix[][MATRIX_SIZE]) {
  T determinant = calculateDeterminant(matrix, MATRIX_SIZE);
  if (determinant == 0) {
    std::cout << "Inverse of Entered Matrix not possible\n";
  } else {
    calculateCofactor(matrix, MATRIX_SIZE);
  }
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
    std::cout << "\n\n";
    printMatrix(matrix);
  }
  return 0;
}



