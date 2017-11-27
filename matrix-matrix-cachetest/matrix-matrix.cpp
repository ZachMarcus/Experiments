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


template <class T>
void makeConsecutive(T** dest, T* source, unsigned int size) {
  for (int i = 0; i < size; i++) {
    dest[i] = source + i * size;
  }
  return;
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


  int** a = new int*[matrixSize];
  int* aData = new int[matrixSize * matrixSize];
  makeConsecutive(a, aData, matrixSize);
  int** b = new int*[matrixSize];
  int* bData = new int[matrixSize * matrixSize];
  makeConsecutive(b, bData, matrixSize);
  int** c = new int*[matrixSize];
  int* cData = new int[matrixSize * matrixSize];
  makeConsecutive(c, cData, matrixSize);

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


