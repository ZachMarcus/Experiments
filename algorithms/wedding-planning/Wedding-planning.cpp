#include <iostream>
#include <vector>
#include <limits>

const int max_int = std::numeric_limits<int>::max();


class City {
public:
  City(int cityNumberIn, int weightIn) {
    cityNumber = cityNumberIn;
    weight = weightIn;
  }
  int cityNumber;
  int weight;
};


void swap(City city1, City city2) {
  std::swap(city1.weight, city2.weight);
  std::swap(city1.cityNumber, city2.cityNumber);
  return;
}


class MinHeap {
public:
  MinHeap() {
    length = 0;
  }
  ~MinHeap() {}
  void insert(int cityNumber, int weight) {
    values.push_back(City(cityNumber, weight));
    bubbleUp(length);
    length++;
    return;
  }

  void insert(City city) {
    values.push_back(city);
    bubbleUp(length);
    length++;
    return;
  }

  City top() {
    return values.at(0);
  }

  void pop() {
    if (length == 0) {return;}
    length--;
    values.at(0) = values.at(length);
    values.pop_back();
    bubbleDown(0);
    return;
  }

  bool isEmpty() {
    if (length == 0) {
      return true;
    }
    return false;
  }


private:
  int length;
  std::vector<City> values;
  void heapify() {
    for (int i = length - 1; i >= 0; --i) {
      bubbleDown(i);
    }
  }

  void bubbleDown(int vertex) {
    int leftChildInd = 2 * vertex + 1;
    int rightChildInd = 2 * vertex + 2;
    int minVertex = vertex;
    // check if we're at a leaf
    if (leftChildInd >= length) {return;}

    if (values.at(vertex).weight > values.at(leftChildInd).weight) {
      minVertex = leftChildInd;
    }

    if ((rightChildInd < length) && (values.at(minVertex).weight > values.at(rightChildInd).weight)) {
      minVertex = rightChildInd;
    }

    if (minVertex != vertex) { // then we perform the swap
      swap(values.at(vertex), values.at(minVertex));
      bubbleDown(minVertex);
    }
    return;
  }


  void bubbleUp(int vertex) {
    if (vertex == 0) {return;}
    int maxVertex = (vertex - 1) / 2;
    if (values.at(maxVertex).weight > values.at(vertex).weight) { // then we perform the swap
      swap(values.at(vertex), values.at(maxVertex));
      bubbleUp(maxVertex);
    }
    return;
  }

};





int main(int argc, char**argv) {

  int numCities, numEdges, rootCity;
  std::cin >> numCities >> numEdges >> rootCity;
  // Choosing to have an empty 0th row and column for now
  // Construct an adjacency list describing connectivity and edge weights
  std::vector<std::vector<int>> adjacencyList(numCities+1, std::vector<int>(numCities+1));
  int tempU, tempV, tempD;
  for (unsigned int i = 0; i < numEdges; i++) {
    std::cin >> tempU >> tempV >> tempD;
    adjacencyList.at(tempU).at(tempV) = tempD;
  }

  std::vector<int> distFromSource(numCities+1, max_int);
  distFromSource[rootCity] = 0;

  std::cout << "dists\n";
  for (auto i : distFromSource) {
    std::cout << i << " ";
  }
  std::cout << std::endl;

  std::vector<int> predecessors(numCities+1);
  std::vector<bool> visited(numCities+1);
  MinHeap minHeap;

  // don't insert the root city the same
  for (int i = 1; i <= numCities; i++) {
    if (i != rootCity) {
      minHeap.insert(i, max_int);
    } else {
      minHeap.insert(i, 0);
    }
  }

  std::vector<int> connectedCities;
  while (!minHeap.isEmpty()) {
    connectedCities.clear();
    City tempCity = minHeap.top();
    minHeap.pop();
    // mark the current City as visited
    visited.at(tempCity.cityNumber) = true;
    // now find edges from the adjacency list
    for (int i = 1; i <= numCities; i++) {
      if (adjacencyList.at(tempCity.cityNumber).at(i) != 0) {
        connectedCities.push_back(i);
      }
    }

    for (auto i : connectedCities) {
      std::cout << "City " << i << 
      int currentWeight = distFromSource[i];
      int newWeight = adjacencyList.at(tempCity.cityNumber).at(i) + distFromSource[tempCity.cityNumber];
      if (newWeight < currentWeight) {
        std::cout << "Updating City " << i << " from distance " << currentWeight << " to distance " << newWeight << std::endl;
        predecessors[i] = tempCity.cityNumber;
        distFromSource[i] = newWeight;
        // now need to update the minHeap somehow
      }
    }
  }



  //MinHeap myHeap;
  //myHeap.printStatus();

  
  return 0;
}


