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
T calculateDeterminant(T matrix[][MATRIX_SIZE], int size) {
  T temp = 1;
  std::cout << "Determinant func, size: " << size << std::endl;
  T determinant = 0;
  T tempMatrix[MATRIX_SIZE][MATRIX_SIZE];
  if (size == 1) {
    return matrix[0][0];
  } else {
    for (int c = 0; c < size; c++) {
      int m = 0;
      int n = 0;
      for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
          tempMatrix[i][j] = 0;
          if (i != 0 && j!= c) {
            tempMatrix[m][n] = matrix[i][j];
            if (n < (size - 2)) {
              n++;
            } else {
              n = 0;
              m++;
            }
          }
        }
      }
      determinant = determinant + temp * (matrix[0][c] * calculateDeterminant(tempMatrix, size-1));
      temp = -1 * temp;
    }
  }
  return determinant;
}


template<class T>
void calculateCofactor(T array[MATRIX_SIZE][MATRIX_SIZE], T factor) {
  T tempArray[MATRIX_SIZE][MATRIX_SIZE], factors[MATRIX_SIZE][MATRIX_SIZE];
  int m, n;
  for (int q = 0; q < factor; q++) {
    for (int p = 0; p < factor; p++) {
      m = 0;
      n = 0;
      for (int i = 0; i < factor; i++) {
        for (int j = 0; j < factor; j++) {
          tempArray[i][j] = 0;
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
void calculateTranspose(T matrix[MATRIX_SIZE][MATRIX_SIZE], T factors[MATRIX_SIZE][MATRIX_SIZE], float r) {
  T tempMatrix[MATRIX_SIZE][MATRIX_SIZE];
  T inverseMatrix[MATRIX_SIZE][MATRIX_SIZE];
  T d;
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < r; j++) {
      tempMatrix[i][j] = factors[j][i];
    }
  }
  d = calculateDeterminant(matrix, r);
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < r; j++) {
      std::cout << "Matrix[i][j] = " << matrix[i][j] << " tempMatrix[i][j] = " << tempMatrix[i][j] << " d = " << d << std::endl;
      matrix[i][j] = tempMatrix[i][j] / d;
    }
  }

  // NOTE: chose to expand out the for loops rather than doing it in one loop
  //       to improve performance in accessing memory where matrices stored

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


void betterMatrixInverse(int matrix[][MATRIX_SIZE]) {
  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = MATRIX_SIZE; j < 2 * MATRIX_SIZE; j++) {
      if (i == (j - MATRIX_SIZE)) {
        matrix[i][j] = 1;
      } else {
        matrix[i][j] = 0;
      }
    }
  }
  
  int temp; 
  for (int i = 0; i < MATRIX_SIZE; i++) {
    temp = matrix[i][i];
    for (int j = 0; j < 2 * MATRIX_SIZE; j++) {
      matrix[i][j] = matrix[i][j] / temp;
    }
  }
}



void gaussJordanInverse(int matrix[][MATRIX_SIZE]) {
  float inverse[MATRIX_SIZE][MATRIX_SIZE];
  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      inverse[i][j] = (float)matrix[i][j];
    }
  }
  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < 2 * MATRIX_SIZE; j++) {
      if (j == (i + MATRIX_SIZE)) {
        inverse[i][j] = 1;
      }
    }
  }
  // partial pivoting
  float d;
  for (int i = MATRIX_SIZE; i > 1; i--) {
    if (matrix[i - 1][1] < inverse[i][1]) {
      for (int j = 0; j < MATRIX_SIZE * 2; j++) {
        d = inverse[i][j];
        inverse[i][j] = inverse[i-1][j];
        inverse[i-1][j] = d;
      }
    }
  }
  // reducing to diagonal matrix:
  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      if (j != i) {
        d = inverse[j][i] / inverse[i][i];
        for (int k = 1; k < MATRIX_SIZE * 2; k++) {
          inverse[j][k]-= inverse[i][k] * d;
        }
      }
    }
  }
  // reducing to unit matrix
  for (int i = 0; i < MATRIX_SIZE; i++) {
    d = inverse[i][i];
    for (int j = 0; j < MATRIX_SIZE * 2; j++) {
      inverse[i][j] = inverse[i][j] / d;
    }
  }

  printMatrix(inverse);

  return;
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
  gaussJordanInverse(matrix);
  return 0;
}



