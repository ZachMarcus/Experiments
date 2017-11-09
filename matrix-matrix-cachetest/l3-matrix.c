#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// time in milliseconds
double CLOCK() {
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC,  &t);
  return (t.tv_sec * 1000)+(t.tv_nsec*1e-6);
}


int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: ./matrix-3 [MatrixDimension]\n");
    return -1;
  }
  int matrixSize = atoi(argv[1]);

  double start, finish, total;

  // favor a dynamic allocation to allow for command-line
  // determination of matrix size
  int **a = (int**)malloc(matrixSize * sizeof(int*));
  int **b = (int**)malloc(matrixSize * sizeof(int*));
  int **c = (int**)malloc(matrixSize * sizeof(int*));
  for (int size = 0; size < matrixSize; size++) {
    a[size] = (int*)malloc(matrixSize * sizeof(int));
    b[size] = (int*)malloc(matrixSize * sizeof(int));
    // calloc - this needs to be 0 initialized anyway, may as well do it here
    c[size] = (int*)calloc(matrixSize, sizeof(int));
  }

  // fill array a with meaningful values
  for (int i = 0; i < matrixSize; i++) {
    for (int j = 0; j < matrixSize; j++) {
      a[i][j] = i + j;
    }
  }

  // fill array b with meaningful values
  for (int i = 0; i < matrixSize; i++) {
    for (int j = 0; j < matrixSize; j++) {
      b[i][j] = i + j;
    }
  }
     
  start = CLOCK();

  // do the actual matrix multiplication
  for (int i = 0; i < matrixSize; i++) {
    for (int j = 0; j < matrixSize; j++) {
      for (int k = 0; k < matrixSize; k++) {
        c[i][j] += a[i][k] * b[k][j];
      }
    }
  }

  finish = CLOCK();
  total = finish - start;
  printf("%f\n", total);

  return 0;
}


