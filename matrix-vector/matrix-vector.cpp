#include <iomanip>
#include <random>
#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#include <omp.h>

//#define PRINT

/*
 * Class to contain a matrix of size nxn and a vector of size n
 * Fills them with pseudo-random numbers
 * Performs matrix-vector multiplication in serial or OpenMP threads
 *
 */
class MatrixVector {
public:
  MatrixVector(unsigned int dimension) {
    // create random number generator
    dim = dimension;
    std::chrono::time_point<std::chrono::system_clock> start;
    start = std::chrono::system_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::seconds>(start.time_since_epoch());
    mt = std::mt19937(time.count());
    dist = std::uniform_int_distribution<int>(0,100);

    matrix = std::vector<std::vector<int>>(dimension, std::vector<int>(dimension));
    vec = std::vector<int>(dimension);
    for (unsigned int i = 0; i < dimension; i++) {
      for (unsigned int j = 0; j < dimension; j++) {
        matrix.at(i).at(j) = randomNumber();
      }
    }
    for (unsigned int i = 0; i < dimension; i++) {
      vec.at(i) = randomNumber();
    }
  }

  ~MatrixVector() {}

  std::string getState() {
    std::stringstream buf;
    buf << "[\n";
    for (unsigned int i = 0; i < dim; i++) {
      buf << "[";
      for (unsigned int j = 0; j < dim; j++) {
        buf << matrix.at(i).at(j) << " ";
      }
      buf << "]\t" << vec.at(i) << "\n";
    }
    buf << "]\n";
    return buf.str();
  }

  std::string runMultiplySingular() {
#ifdef PRINT
    std::stringstream buf;
#endif
    std::vector<int> result(dim);
    for (unsigned int i = 0; i < dim; i++) {
      for (unsigned int j = 0; j < dim; j++) {
        result.at(i) += matrix.at(i).at(j)*vec.at(j);
      }
    }
#ifdef PRINT
    for (auto i : result) {
      buf << i << " ";
    }
    return buf.str();
#endif
#ifndef PRINT
    return "Complete";
#endif
  }

  std::string runMultiplyOpenMp() {
#ifdef PRINT
    std::stringstream buf;
#endif
    std::vector<int> result(dim);
    std::cout << "Available processors: " << omp_get_max_threads() << "\n";
    #pragma omp parallel
    {
      std::stringstream tempBuf;
      tempBuf << "Thread ID: " << omp_get_thread_num() << std::endl;
      std::cout << tempBuf.str();
      #pragma omp for
      for (unsigned int i = 0; i < dim; i++) {
        int temp = 0;
        #pragma omp parallel for reduction(+:temp)
        for (unsigned int j = 0; j < dim; j++) {
          temp += matrix.at(i).at(j) * vec.at(j);
        }
        result.at(i) = temp;
      }
    }
#ifdef PRINT
    buf << "\n";
    for (auto i : result) {
      buf << i << " ";
    }
    buf << "\n";
    return buf.str();
#endif
#ifndef PRINT
    return "Complete";
#endif
  }


private:
  int dim;
  std::vector<std::vector<int>> matrix;
  std::vector<int> vec;
  std::mt19937 mt;
  std::uniform_int_distribution<int> dist;

  int randomNumber() {
    return dist(mt);
  }

};


int main(int argc, char **argv) {

  if (argc != 4) {
    std::cout << "Usage: ./matrix-vector [dimension] [shouldSerial] [shouldOpenMP]" << std::endl;
    exit(-1);
  }
  int size = atoi(argv[1]);
  int shouldSerial = atoi(argv[2]);
  int shouldOpenMp = atoi(argv[3]);
  std::cout << "Initializing matrix and vector with dimension " << size << std::endl;

  std::chrono::steady_clock::time_point start, end;
  MatrixVector matVec = MatrixVector(size);
  std::string serialResult, openMpResult;
  if (shouldSerial) {
    start = std::chrono::steady_clock::now();
    serialResult = matVec.runMultiplySingular();
    end = std::chrono::steady_clock::now();
    std::cout << "Singular took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms.\n";
  }
  if (shouldOpenMp) {
    start = std::chrono::steady_clock::now();
    openMpResult = matVec.runMultiplyOpenMp();
    end = std::chrono::steady_clock::now();
    std::cout << "OpenMP took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms.\n";
  }
  
  if (shouldSerial) {
    std::cout << "Serial result: \n" << serialResult << "\n";
  }
  if (shouldOpenMp) {
    std::cout << "OpenMP result: \n" << openMpResult << "\n";
  }
  return 0;
}

