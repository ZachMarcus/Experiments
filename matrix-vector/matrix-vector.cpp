#include <iomanip>
#include <random>
#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#include <omp.h>

class MatrixVector {
public:
  MatrixVector(unsigned int dimension) {
    // create random number generator
    dim = dimension;
    std::chrono::time_point<std::chrono::system_clock> start;
    start = std::chrono::system_clock::now();
    auto tempeh = std::chrono::duration_cast<std::chrono::seconds>(start.time_since_epoch());
    mt = std::mt19937(tempeh.count());
    dist = std::uniform_int_distribution<int>(0,1);

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
    std::stringstream buf;
    std::vector<int> result(dim);
    for (unsigned int i = 0; i < dim; i++) {
      for (unsigned int j = 0; j < dim; j++) {
        result.at(i) += matrix.at(i).at(j)*vec.at(j);
      }
    }
    for (auto i : result) {
      buf << i << " ";
    }
    return buf.str();
  }

/*
  std::string runMultiplyOpenMp() {
    std::stringstream buf;
    std::vector<int> result(dim);
    std::cout << "Available processors: " << omp_get_max_threads() << "\n";
    std::stringstream tempBuf;
    tempBuf << "Thread ID: " << omp_get_thread_num() << std::endl;
    std::cout << tempBuf.str();
    for (unsigned int i = 0; i < dim; i++) {
      int temp = 0;
      #pragma omp parallel for reduction(+:temp)
      for (unsigned int j = 0; j < dim; j++) {
        temp += matrix.at(i).at(j) * vec.at(j);
      }
      result.at(i) = temp;
    }
    buf << "\n";
    for (auto i : result) {
      buf << i << " ";
    }
    buf << "\n";
    return buf.str();
  }
*/

  std::string runMultiplyOpenMp() {
    std::stringstream buf;
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
    buf << "\n";
    for (auto i : result) {
      buf << i << " ";
    }
    buf << "\n";
    return buf.str();
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

  if (argc != 3) {
    std::cout << "Usage: ./matrix-vector [dimension] [print]" << std::endl;
    exit(-1);
  }
  int size = atoi(argv[1]);
  int print = atoi(argv[2]);
  std::cout << "Initializing matrix and vector with dimension " << size;

  MatrixVector matVec = MatrixVector(size);

  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  std::string singularOutput = matVec.runMultiplySingular();
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << "Singular took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms.\n";

  start = std::chrono::steady_clock::now();
  std::string openMpOutput = matVec.runMultiplyOpenMp();
  end = std::chrono::steady_clock::now();
  std::cout << "OpenMP took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms.\n";
  if (print) {
    std::cout << "Singular result: \n";
    std::cout << singularOutput << std::endl;
    std::cout << "OpenMP result: \n";
    std::cout << openMpOutput << std::endl;
  }

  return 0;
}

