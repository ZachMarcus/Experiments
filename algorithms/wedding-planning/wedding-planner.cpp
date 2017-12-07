


#include <iostream>
#include <vector>
#include <unordered_map>


const int MAX_INT = 500000;


class City {
public:
    City(int inNumber, int inWeight) {
        number = inNumber;
        weight = inWeight;
    }
    ~City() {}

    bool operator < (City other) const {
        return weight < other.weight;
    }
    bool operator > (City other) const {
        return weight > other.weight;
    }

    int number;
    int weight;
};



class MinHeap {
public:
    MinHeap() {
        length = 0;
        cities.clear();
        citiesMapping.clear();
    }
    ~MinHeap() {}

    std::unordered_map<int,int> citiesMapping;
    std::vector<City> cities;
    int length;

    void swap(int leftIndex, int rightIndex) {
        int tempWeight = cities[leftIndex].weight;
        int tempNumber = cities[rightIndex].number;
        cities[leftIndex].weight = cities[rightIndex].weight;
        cities[leftIndex].number = cities[rightIndex].number;
        cities[rightIndex].weight = tempWeight;
        cities[rightIndex].number = tempNumber;
        // Now swap the keys
        int numberOfLeft = cities[leftIndex].number;
        int numberOfRight = cities[rightIndex].number;
        int tempMapping = citiesMapping[numberOfLeft];
        citiesMapping[numberOfLeft] = citiesMapping[numberOfRight];
        citiesMapping[numberOfRight] = tempMapping;
        return;
    }

    void update(int cityNumber, int newWeight) {
        int index = citiesMapping[cityNumber];
        cities[index].weight = newWeight;
        bubbleUp(index);
        return;
    }

    void insert(City city) {
        cities.push_back(city);
        citiesMapping[city.number] = length;
        bubbleUp(length);
        length++;
        return;
    }

    City top() {
        if (length == 0) {
            return City(-1,-1);
        }
        return cities.at(0);
    }

    void pop() {
        if (length == 0) {
            return;
        }
        length--;
        City tempCity = cities.at(0);
        citiesMapping.erase(tempCity.number);
        cities[0] = cities[length];
        citiesMapping[cities[0].number] = 0;
        cities.pop_back();
        bubbleDown(0);
        return;
    }
        
    bool isEmpty() {
        return length == 0;
    }

    void heapify() {
        for (int i = length-1; i >= 0; i--) {
            bubbleDown(i);
        }
        return;
    }

    void bubbleDown(int vertex) {
        int leftChild = 2 * vertex + 1;
        int rightChild = 2 * vertex + 2;
        int minVertex = vertex;
        // check if we're at a leaf
        if (leftChild >= length) {
            return;
        }
        if (cities[vertex].weight > cities[leftChild].weight) {
            minVertex = leftChild;
        }
        if ((rightChild < length) && cities[minVertex].weight > cities[rightChild].weight) {
            minVertex = rightChild;
        }
        if (minVertex != vertex) {
            swap(minVertex, vertex);
            bubbleDown(minVertex);
        }
        return;
    }

    void bubbleUp(int vertex) {
        if (vertex == 0) {
            return;
        }
        int maxVertex = (int)((vertex - 1) / 2);
        if (cities[maxVertex].weight > cities[vertex].weight) {
            swap(maxVertex, vertex);
            bubbleUp(maxVertex);
        }
        return;

    }
};




class WeddingPlanner {
public:
    WeddingPlanner() {
        visitedCities.clear();
        distances.clear();
        outgoingVertices.clear();
        minHeap = MinHeap();
        return;
    }

    ~WeddingPlanner() {}

    std::unordered_map<int, bool> visitedCities;
    std::unordered_map<int, int> distances;
    std::unordered_map<int, std::vector<int>> outgoingVertices;
    MinHeap minHeap;

    void plan() {
        int numCities, numEdges, rootCity;
        std::cin >> numCities >> numEdges >> rootCity;
        std::cout << numCities << " " << numEdges << " " << rootCity << std::endl;
        for (int vertex = 1; vertex <= numCities; vertex++) {
            std::unordered_map<int,int> innerDict;
            std::pair<int, std::unordered_map<int,int>> temp = {vertex, innerDict};
            outgoingVertices.insert(temp);
        }

        int srcCity, dstCity, pathWeight;
        for (int x = 0; x < numEdges; x++) {
            std::cin >> srcCity >> dstCity >> pathWeight;
            outgoingVertices[dstCity].insert(std::make_pair<int,int>(srcCity, pathWeight));
        }

        // Initialization
        for (int city = 1; city <= numCities; city++) {
            visitedCities.insert(std::make_pair<int,bool>(city, false));
            distances.insert(std::make_pair<int,int>(city, INT_MAX));
            City tempCity = City(city, INT_MAX);
            minHeap.insert(tempCity);
        }

        distances[rootCity] = 0;
        minHeap.update(rootCity, 0);

        while (!minHeap.isEmpty()) {
            City city = minHeap.top();
            visitedCities[city.number] = true;
            for (auto keyValue : outgoingVertices[city.number]) {
                int vertex = keyValue.first;
                int weight = keyValue.second;
                int newDist = distances[city.number] + weight;
                int oldDist = distances[vertex];
                if (newDist < oldDist) {
                    distances[vertex] = newDist;
                    minHeap.update(vertex, newDist);
                }
            }
            minHeap.pop();
        }

        // Now output the results
        std::string result("");
        for (int city = 1; city <= numCities; city++) {
            if (distances[city] == MAX_INT) {
                result += "-1 ";
            } else {
                result += std::to_string(distances[city]) + " ";
            }
        }
        result.pop_back();
        std::cout << result;
    }
};




int main() {
    WeddingPlanner planner;
    planner.plan();
    return 0;
}




 






























