

#include <iostream>
#include <chrono>
#include <cstdlib>
#include <random>
#include <omp.h>

const int MATRIX_SIZE = 1000;

//https://en.wikipedia.org/wiki/Gaussian_elimination
// OR Look into CHOL2inv from Fortran
// OR USE DPOTRI from FORTRAN


template<class T>
T abs(T input) {
  if (input < 0) {
    return input * -1;
  }
  return input;
}

template<class T>
void printMatrix(T** matrix) {
  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      std::cout << matrix[i][j] << " ";
    }
    std::cout << "\n";
  }
  return;
}

template<class T>
void calculateGaussian(T** inputMatrix, int index[]) {
  T factors[MATRIX_SIZE];
  // initialize index array 
  for (unsigned int i = 0; i < MATRIX_SIZE; i++) {
    index[i] = i;
  }

  // find the rescaling factors, one from each row
 // #pragma omp parallel for
  for (unsigned int i = 0; i < MATRIX_SIZE; ++i) {
    T factor = 0.;
    //#pragma omp parallel for reduction(max:factor)
    for (unsigned int j = 0; j < MATRIX_SIZE; ++j) {
      T factorTemp = abs(inputMatrix[i][j]);
      if (factorTemp > factor) {
        factor = factorTemp;
      }
    }
    factors[i] = factor;
  }
  
  // search pivoting element from each column
  int k = 0;
//  #pragma omp parallel
  for (unsigned int j = 0; j < MATRIX_SIZE - 1; ++j) {
    T factor = 0;
//    #pragma omp do private(factorTemp) schedule(runtime)
    for (unsigned int i = j; i < MATRIX_SIZE; ++i) {
      T factorTemp = abs(inputMatrix[i][j]);
      factorTemp = factorTemp / factors[index[i]];
      if (factorTemp > factor) {
        factor = factorTemp;
        k = i;
      }
    }

    // interchange rows according to the pivoting order
    int iTemp = index[j];
    index[j] = index[k];
    index[k] = iTemp;
    for (unsigned int i = j + 1; i < MATRIX_SIZE; ++i) {
      T jTemp = inputMatrix[index[i]][j] / inputMatrix[index[j]][j];
      // record pivoting ratios below the diagonal
      inputMatrix[index[i]][j] = jTemp;
      // Modify other elements accordingly
      for (unsigned int l = j + 1; l < MATRIX_SIZE; ++l) {
        inputMatrix[index[i]][l] = inputMatrix[index[i]][l] - jTemp * inputMatrix[index[j]][l];
      }
    }
  }
}



template <class T>
void invertMatrix(T** inputMatrix, T** outputMatrix, T** tempMatrix) {
  int index[MATRIX_SIZE];

  auto start = std::chrono::system_clock::now();
  // transform the matrix into an upper triangle
  calculateGaussian(inputMatrix, index);
  auto end = std::chrono::system_clock::now();
  std::cout << "GaussTime: " << (end-start).count() << "\n";

  start = std::chrono::system_clock::now();
  // Update matrixB[i][j] with new ratios stored
  // NOTE: Should consider using blocking here for better spatial locality
  #pragma omp parallel for
  for (unsigned int i = 0; i < MATRIX_SIZE - 1; ++i) {
    for (unsigned int j = i + 1; j < MATRIX_SIZE; ++j) {
      for (unsigned int k = 0; k < MATRIX_SIZE; ++k) {
        tempMatrix[index[j]][k] = tempMatrix[index[j]][k] - inputMatrix[index[j]][i] * tempMatrix[index[i]][k];
      }
    }
  }
  end = std::chrono::system_clock::now();
  std::cout << "RatioLoop: " << (end-start).count() << "\n";

  start = std::chrono::system_clock::now();
  // Perform backward substitutions
  #pragma omp parallel for //num_threads(16)
  for (unsigned int i = 0; i < MATRIX_SIZE; ++i) {
    outputMatrix[MATRIX_SIZE - 1][i] = tempMatrix[index[MATRIX_SIZE - 1]][i] / inputMatrix[index[MATRIX_SIZE - 1]][MATRIX_SIZE - 1];
    for (int j = MATRIX_SIZE - 2; j >= 0; --j) {
      outputMatrix[j][i] = tempMatrix[index[j]][i];
      for (unsigned int k = j + 1; k < MATRIX_SIZE; ++k) {
        outputMatrix[j][i] = outputMatrix[j][i] - inputMatrix[index[j]][k] * outputMatrix[k][i];
      }
      outputMatrix[j][i] = outputMatrix[j][i] / inputMatrix[index[j]][j];
    }
  }
  end = std::chrono::system_clock::now();
  std::cout << "BackSubst: " << (end - start).count() << "\n";
  return; 
} 


template <class T>
void validateMatrix(T** inputMatrix, T** outputMatrix) {
  T** validateMatrix = (T**)malloc(MATRIX_SIZE * sizeof(T*));
  validateMatrix[0] = (T*)malloc(MATRIX_SIZE*MATRIX_SIZE);
  for (int i = 1; i < MATRIX_SIZE; i++) {
    validateMatrix[i] = validateMatrix[0] + (MATRIX_SIZE * i);
  }
  for (unsigned int i = 0; i < MATRIX_SIZE; i++) {
    for (unsigned int j = 0; j < MATRIX_SIZE; j++) {
      validateMatrix[i][j] = 0;
      for (unsigned int k = 0; k < MATRIX_SIZE; k++) {
        validateMatrix[i][j] += inputMatrix[i][k] * outputMatrix[k][j];
      }
    }
  }
  std::cout << "Validation of matrix: " << std::endl;
  printMatrix(validateMatrix);
}

template <class T>
void initMatrix(T** matrix) {
  #pragma omp parallel for
  for (int i = 0; i < MATRIX_SIZE; i++) {
    std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<T> distribution(0, 1000);
    for (int j = 0; j < MATRIX_SIZE; j++) {
      matrix[i][j] = distribution(generator);
    }
  }
  return;
}

template <class T>
void makeConsecutive(T** dest, T* source) {
  for (int i = 0; i < MATRIX_SIZE; i++) {
    dest[i] = source + i * MATRIX_SIZE;
  }
  return;
}


int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Args wrong. Usage: ./matrix-inversion [shouldPrintMatrices]" << std::endl;
    exit(1);
  }

  int shouldPrint = atoi(argv[1]);

  float** inputMatrix = new float*[MATRIX_SIZE];
  float* inputData = new float[MATRIX_SIZE * MATRIX_SIZE];
  makeConsecutive(inputMatrix, inputData);

  float** outputMatrix = new float*[MATRIX_SIZE];
  float* outputData = new float[MATRIX_SIZE * MATRIX_SIZE];
  makeConsecutive(outputMatrix, outputData);

  float** tempMatrix = new float*[MATRIX_SIZE];
  float* tempData = new float[MATRIX_SIZE * MATRIX_SIZE];
  makeConsecutive(tempMatrix, tempData);
  for (unsigned int i = 0; i < MATRIX_SIZE; i++) {
    tempMatrix[i][i] = 1;
  }


  initMatrix(inputMatrix);
  if (shouldPrint) {
    printMatrix(inputMatrix);
  }
  
  auto start = std::chrono::system_clock::now();
  invertMatrix(inputMatrix, outputMatrix, tempMatrix);
  auto end = std::chrono::system_clock::now();
  std::cout << "Mult Time: " << (end - start).count() << "\n";

  if (shouldPrint) {
    std::cout << "Inverted Matrix" << std::endl;
    printMatrix(outputMatrix);
  }

  if (shouldPrint) {
    validateMatrix(inputMatrix, outputMatrix);
  }

  return 0;
}



