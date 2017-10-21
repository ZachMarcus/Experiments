#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// things that have to happen:
// mark all odd numbers true except for 1
// remember to not do anything with array[0] or array[1]
//     those should be set to false

//#define SHOULD_PRINT 0

pthread_barrier_t barrierSync;


typedef struct threadInfo {
  int maxThreads; // how many threads will be running this
  int threadNum; // number for this thread, potentially not needed
  int largestNumber; // parsed from stdin, largestNumber to check for prime
  int* currentPrime; // current value in the vector to be checking
  std::vector<bool>* primeVector; // reference to vector of primes
  pthread_mutex_t *lock; // how to lock
  // the odd numbers to mark true are started by:
  // (largestNumber / maxThreads) * threadNum 
} threadInfo;


void sieveVector(std::vector<bool>* primeVector, int max, int baseVal) {
  for (int i = 3; i * baseVal <= max; i+=2) {
    primeVector->at(i * baseVal) = false;
  }
  return;
}


void *sieveWorker(void* input) {//struct threadInfo *threadInf) {
  struct threadInfo *threadInf = (threadInfo*)input;
#ifdef SHOULD_PRINT
  std::cout << "In Worker " << threadInf->threadNum << std::endl;
#endif // PRINT
  int largestNumber = threadInf->largestNumber;
  int maxThreads = threadInf->maxThreads;
  int threadNum = threadInf->threadNum;
 
  int arraySize = largestNumber+1;
  int chunkSize = arraySize / maxThreads;
  int start = threadNum * chunkSize;
  int end = start + chunkSize - 1;
  if (threadNum == maxThreads - 1) {
    end = largestNumber;
  }
  if (start < 3) {
    start = 3;
  }
  if (start % 2 == 0) {
    start++;
  }

  std::vector<bool>* vec = threadInf->primeVector;

  for (int i = start; i <= end; i+=2) {
    vec->at(i) = true;
  }


  pthread_barrier_wait(&barrierSync);

  // we can save work by not checking anything larger than sqrt(largest)
  int maxCheck = sqrt(largestNumber);
  int temp;
  do {
    // lock to get next multiplier in our sieve thing
    pthread_mutex_lock(threadInf->lock);
    temp = *(threadInf->currentPrime);
    *(threadInf->currentPrime) += 2;
    // unlock now we've incremented currentPrime
    pthread_mutex_unlock(threadInf->lock);
    if (temp <= maxCheck) {
      // check to see if it's still true, only do work if so
      if (threadInf->primeVector->at(temp)) {
        sieveVector(threadInf->primeVector, largestNumber, temp);
      }
    } else {
      return NULL;
    }
  } while (1);

  return 0;
}


int main(int argc, char **argv) {
  if (argc != 3) {
    std::cout << "Usage: ./sieve [numThreads] [largestNumber]" << std::endl;
    return 1;
  }
  //int **ret;
  int numThreads = atoi(argv[1]);
  int largestNumber = atoi(argv[2]);
  std::vector<bool> possibleNums(largestNumber+1); // all set to false
  std::vector<pthread_t> threads(numThreads);
  std::vector<threadInfo> threadInf(numThreads);

  int currPrime = 3;
  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
  pthread_barrier_init(&barrierSync, NULL, numThreads);

  for (int i = 0; i < numThreads; i++) {
    threadInf.at(i).maxThreads = numThreads;
    threadInf.at(i).threadNum = i;
    threadInf.at(i).largestNumber = largestNumber;
    threadInf.at(i).currentPrime = &currPrime;
    threadInf.at(i).primeVector = &possibleNums;
    threadInf.at(i).lock = &lock;
#ifdef SHOULD_PRINT
    std::cout << "Creating thread " << i << std::endl;
#endif // PRINT
    pthread_create(&threads.at(i), NULL, sieveWorker, (void*)&threadInf.at(i));
  }
  // cleanup
#ifdef SHOULD_PRINT
  std::cout << "Cleaning up threads" << std::endl;
#endif // PRINT
  for (int i = 0; i < numThreads; i++) {
    pthread_join(threads.at(i),NULL);
  }
  pthread_barrier_destroy(&barrierSync);
#ifdef SHOULD_PRINT
  for (int i = 0; i < possibleNums.size(); i++) {
    if (possibleNums.at(i)) {
      std::cout << i << "\n";
    }
  }
#endif // PRINT
  return 0;
}



