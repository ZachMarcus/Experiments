#include <vector>
#include <iostream>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <string>
#include <chrono>

template<typename T>
T taylor(T x, unsigned int termNumber) {
  bool is_double = std::is_same<double, T>::value;
  bool is_float = std::is_same<float, T>::value;
  bool is_int = std::is_same<int, T>::value;
  if (!is_double && !is_float) {
    std::cerr << "Incorrect input of type" << std::endl;
    T ret;
    return ret;
  }

  T ret = 0;
  T power = 1;
  bool positive = true;
  for (int i = 0; i < termNumber; i++) {
    if (positive) {
      ret += pow(x, power) / power;
    } else {
      ret -= pow(x, power) / power;
    }
    positive = !positive;
    power += 2;
  }


  return ret;
}



void testTiming() {
  std::vector<float> floats = {-1.45678,2,-3,4.765,5,6,7,8.5,45};
  std::vector<double> doubles = {-1.45678,2,-3,4.765,5,6,7,8.5,45};
  auto start = std::chrono::system_clock::now();
  for (unsigned int i = 0; i < 10; i++) {
    for (auto i : floats) {
      taylor(i, 15);
    }
  }
  auto end = std::chrono::system_clock::now();  
  std::cout << "Float Time: " << (end-start).count() << "\n";

  start = std::chrono::system_clock::now();
  for (unsigned int i = 0; i < 10; i++) {
    for (auto i : doubles) {
      taylor(i, 15);
    }
  }
  end = std::chrono::system_clock::now();
  std::cout << "Double Time: " << (end-start).count() << "\n";
  
  return;
}


int main(int argc, char **argv) {
  unsigned int numIterations;
  if (argc != 2) {
    std::cerr << "Usage: ./taylor [numIterations]\n";
    return -1;
  } else {
    numIterations = atoi(argv[1]);
  }

  testTiming();
 
  std::vector<float> floats = {-1.45678,.2,-.3,4.765,.5,.6,.7,.85,.45};
  std::vector<float> floatResults(floats.size());
  std::vector<double> doubles = {-1.45678,.2,-.3,4.765,.5,.6,.7,.85,.45};
  std::vector<double> doubleResults(doubles.size());
  std::stringstream temp;
  temp << std::fixed << std::setprecision(64);

  std::cout << "Floats:\n";
  float floatResult;
  for (unsigned int i = 0; i < floats.size(); i++) {
    floatResult = taylor(floats.at(i), numIterations);
    floatResults.at(i) = floatResult;
    temp << floatResult;
    std::cout << temp.str().substr(0,53) << "\n";
    temp.str(std::string()); // clear stream
  }

  std::cout << "\nDoubles:\n";
  double doubleResult;
  for (unsigned int i = 0; i < doubles.size(); i++) {
    doubleResult = taylor(doubles.at(i), numIterations);
    doubleResults.at(i) = doubleResult;
    temp << doubleResult;
    std::cout << temp.str().substr(0,53) << "\n";
    temp.str(std::string()); // clear stream
  }

  std::cout << std::fixed << std::setprecision(16) << "\nDifferences:\n";
  double diffPercent;
  for (unsigned int i = 0; i < doubles.size(); i++) {
    diffPercent = 100 * (doubleResults.at(i) - floatResults.at(i))/doubleResults.at(i);
    std::cout << diffPercent << "\n";
  } 

  std::cout << "\n";


  return 0;
}







