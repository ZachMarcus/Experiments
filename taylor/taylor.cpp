#include <vector>
#include <iostream>
#include <math.h>
#include <iomanip>

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




int main(int argc, char **argv) {
 
  std::vector<float> floats = {1.1, 2.2, 3.3, 4.400004444};
  std::vector<double> doubles = {1.1, 2.2, 3.3, 4.400004444};
  unsigned int numIterations = 28;

  std::cout << std::fixed << std::setprecision(16);

  std::cout << "Floats:\n";
  for (auto i : floats) {
    std::cout << taylor(i, numIterations) << "\t";
  }

  std::cout << "\nDoubles:\n";

  for (auto i : doubles) {
    std::cout << taylor(i, numIterations) << "\t";

  }
  std::cout << std::endl;

  return 0;
}







