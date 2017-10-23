
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <random>
#include <chrono>
#include <thread>

#include <omp.h>

// Approach: Partial ordering. the last philosopher who picks up their left
// fork should instead pick up the right. Whenever a philosopher wants to eat,
// they pick up their left fork. If they cannot pick up their right fork,
// they put it back down. without the last philosopher picking up their 
// right fork first, this might deadlock eventually. Each philosopher
// essentially must pick up the lowest number fork they can.



#define PRINT

bool running = true;

// Class to represent a philosopher
// should somehow maintain the two mutexes required to communicate with
// neighboring philosophers
class Philosopher {
public:
  Philosopher(int numPhil) {
    numPhilosophers = numPhil;
    // C++'s better random number generation
    randomEngine = std::mt19937_64{std::random_device{}()};
    timesEaten = 0;
  }
  
  void doWork() {
    int numIterations = 50;
    int failed;
    int numTries;
    omp_lock_t *tempFork;
    while (timesEaten < 100) {
#ifdef PRINT
      std::cout << "Philosopher #" << num << " is thinking\n";
#endif
      std::uniform_int_distribution<> dist{10,50};
      std::this_thread::sleep_for(std::chrono::milliseconds{dist(randomEngine)});
#ifdef PRINT
      std::cout << "Philosopher #" << num << " is hungry\n";
#endif
     // Allow multiple tries to pick up forks
     numTries = 2;
      do {
        failed = omp_set_lock(leftFork);
        if (numTries > 0) {
          pthread_mutex_trylock(rightFork);
        } else {
          omp_set_lock(rightFork);
        }
        if (failed) {
          omp_unset_lock(leftFork);
          tempFork = leftFork;
          leftFork = rightFork;
          rightFork = tempFork;
          numTries--;
        }
      } while(failed && running);

      if (!failed) {
        timesEaten++;
#ifdef PRINT
        std::cout << "Philosopher #" << num << " is eating with left fork " << num << " and right fork " << (num+numPhilosophers-1) %(numPhilosophers) << "\n";
#endif
        std::uniform_int_distribution<> dist{10,50};
        std::this_thread::sleep_for(std::chrono::milliseconds{dist(randomEngine)});
        omp_unset_lock(rightFork);
        omp_unset_lock(leftFork);
        timesEaten++;
      }
    }
    
    return NULL;
  }

  void set(int setNum, omp_lock_t* left, omp_lock_t* right) {
    num = setNum;
    leftFork = left;
    rightFork = right;
  }

  void printResult() {
    std::cout << "Philosopher number " << num << " ate " << timesEaten << " times." << std::endl;
    return;
  }

private:
  std::mt19937_64 randomEngine;
  omp_lock_t *leftFork;
  omp_lock_t *rightFork;
  int failure;
  int num;
  int numPhilosophers;
  int timesEaten;
};

void printState(int numPhilosophers) {
  std::cout << "Philosophers arranged around a circular table\n";
  std::cout << "Philosopher\tLeftFork\tRightFork\n";
  for (int i = 0; i < numPhilosophers; i++) {
    std::cout << i << "\t\t" << i << "\t\t" << (i+numPhilosophers-1)%numPhilosophers << "\n";
  }
  return;
}



bool dinnerParty(int numPhilosophers) {
  std::vector<Philosopher> philosophers;
  for (unsigned int i = 0; i < numPhilosophers; i++) {
    // create a philosopher
    philosophers.push_back(Philosopher(numPhilosophers));
  }
  // create the forks / mutexes
  std::vector<omp_lock_t> forks(numPhilosophers);
  int failed;
  for (auto fork : forks) {
    failed = omp_init_lock(&fork);
    if (failed) {
      std::cout << "Failed in initializing fork mutexes." << std::endl;
      exit(1);
    }
  }
  
  // create the thread for each philosopher
  #pragma omp parallel num_threads(numPhilosophers)
  {
    int i = omp_get_thread_num();
    philosophers[i].set(i, &forks[i], &forks[(i+1)%numPhilosophers]);
    philosophers[i].doWork();
    (philosophers[i].getThread(), NULL, &Philosopher::doWorkHelp, &philosophers[i]);
  }
#ifdef PRINT
  std::cout << "Created threads for all philosophers" << std::endl;
  printState(numPhilosophers);
  for (auto philosopher : philosophers) {
    philosopher.printResult();
  }
#endif

  for (auto fork : forks) {
    omp_destroy_lock(&fork);
  }

  return true;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Usage:\n./philosophers [numPhilosophers]\n";
    return 1;
  }
  int numPhilosophers = atoi(argv[1]);
  dinnerParty(numPhilosophers);
  return 0;
}

