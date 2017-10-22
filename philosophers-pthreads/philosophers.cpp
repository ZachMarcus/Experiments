
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <random>
#include <chrono>
#include <thread>
// Approach: Partial ordering. the last philosopher who picks up their left
// fork should instead pick up the right. Whenever a philosopher wants to eat,
// they pick up their left fork. If they cannot pick up their right fork,
// they put it back down. without the last philosopher picking up their 
// right fork first, this might deadlock eventually. Each philosopher
// essentially must pick up the lowest number fork they can.

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
  
  void *doWork(void *unused) {
    int numIterations = 50;
    int failed;
    int numTries;
    pthread_mutex_t *tempFork;
    while (true) {
      std::cout << "Philosopher #" << num << " is thinking\n";
      std::uniform_int_distribution<> dist{10,50};
      std::this_thread::sleep_for(std::chrono::milliseconds{dist(randomEngine)});
      std::cout << "Philosopher #" << num << " is hungry\n";
     // Allow multiple tries to pick up forks
     numTries = 2;
      do {
        failed = pthread_mutex_lock(leftFork);
        if (numTries > 0) {
          pthread_mutex_trylock(rightFork);
        } else {
          pthread_mutex_lock(rightFork);
        }
        if (failed) {
          pthread_mutex_unlock(leftFork);
          tempFork = leftFork;
          leftFork = rightFork;
          rightFork = tempFork;
          numTries--;
        }
      } while(failed && running);

      if (!failed) {
        timesEaten++;
        std::cout << "Philosopher #" << num << " is eating with left fork " << num << " and right fork " << (num+numPhilosophers-1) %(numPhilosophers) << "\n";
        std::uniform_int_distribution<> dist{10,100};
        std::this_thread::sleep_for(std::chrono::milliseconds{dist(randomEngine)});
        pthread_mutex_unlock(rightFork);
        pthread_mutex_unlock(leftFork);
      }
    }
    
    return NULL;
  }

  // trick to pass a class method to pthread_create
  static void *doWorkHelp(void *context) {
    return ((Philosopher*)context)->doWork(NULL);
  }

  pthread_t *getThread() {return &thread;}

  void set(int setNum, pthread_mutex_t* left, pthread_mutex_t* right) {
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
  pthread_mutex_t *leftFork;
  pthread_mutex_t *rightFork;
  pthread_t thread;
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
  std::vector<pthread_mutex_t> forks(5);
  int failed;
  for (auto fork : forks) {
    failed = pthread_mutex_init(&fork, NULL);
    if (failed) {
      std::cout << "Failed in initializing fork mutexes." << std::endl;
      exit(1);
    }
  }
  
  // create the thread for each philosopher
  for (unsigned int i = 0; i < philosophers.size(); i++) {
    philosophers[i].set(i, &forks[i], &forks[(i+1)%numPhilosophers]);
    pthread_create(philosophers[i].getThread(), NULL, &Philosopher::doWorkHelp, &philosophers[i]);
  }
  std::cout << "Created threads for all philosophers" << std::endl;
  printState(numPhilosophers);
  for (auto philosopher : philosophers) {
    pthread_join(*philosopher.getThread(), NULL);
  }
  
  for (auto philosopher : philosophers) {
    philosopher.printResult();
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

