#define _POSIX_C_SOURCE 199309L
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
  if (argc != 3) {
    printf("Usage: ./matrix-matrix [MatrixDimension] [TileSize]\n");
    return -1;
  }
  int matrixSize = atoi(argv[1]);
  int tileSize = atoi(argv[2]);

  int i,j,k,jj,kk,en;
  double start, finish, total, sum;

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
  for (i = 0; i < matrixSize; i++) {
    for (j = 0; j < matrixSize; j++) {
      a[i][j] = i + j;
    }
  }

  // fill array b with meaningful values
  for (i = 0; i < matrixSize; i++) {
    for (j = 0; j < matrixSize; j++) {
      b[i][j] = i + j;
    }
  }
     
  en = tileSize;
  start = CLOCK();

  // do the actual matrix multiplication
  for (kk = 0; kk < en; kk += tileSize) {
    for (jj = 0; jj < en; jj += tileSize) {
      for (i = 0; i < matrixSize; i++) {
        for (j = jj; j < jj + tileSize; j++) {
          // reduction
          sum = c[i][j];
          for (k = kk; k < kk + tileSize; k++) {
            sum+= a[i][j] * b[k][j];
          }
          c[i][j] = sum;
        }
      }
    }
  }

  finish = CLOCK();
  total = finish - start;
  printf("%f\n", total);

  return 0;
}


