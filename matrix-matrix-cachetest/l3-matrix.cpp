
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
  if (argc != 2) {
    printf("Usage: ./matrix-3 [MatrixDimension]\n");
    return -1;
  }

  //printf("sizeofint= %d\n",sizeof(int));

  int matrixSize = atoi(argv[1]);

  double start, finish, total;

  // favor a dynamic allocation to allow for command-line
  // determination of matrix size

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

  // fill array c with 0s
  for (int i = 0; i < matrixSize; i++) {
    for (int j = 0; j < matrixSize; j++) {
      c[i][j] = i + j;
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


