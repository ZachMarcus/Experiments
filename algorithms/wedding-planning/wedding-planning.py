#!/usr/bin/python3

import sys


#for line in sys.stdin:
#    print(line)

def swap_cities(city1, city2):
    city3 = (city1.number, city1.weight)
    city1.number = city2.number
    city1.weight = city2.weight
    city2.number = city3.first
    city2.weight = city3.second


class City:
    def __init__(self, number, weight):
        self.number = number
        self.weight = weight
 

class Min_Heap:
    def __init__(self):
        self.length = 0
        self.cities = []

    def insert(self, city):
        self.cities.push_back(city)
        self.bubble_up(self.length)
        self.length += 1
        return

    def top(self):
        if self.length == 0:
            print('Error, cities is empty')
            return
        return cities[0]

    def pop(self):
        if self.length == 0:
            return
        length -= 1
        temp_city = self.cities[0]
        self.cities[0] = self.cities[length]
        self.cities = self.cities[:-1]
        self.bubble_down(0)
        return

    def is_empty(self):
        return self.length == 0

    def heapify(self):
        for i in range(self.length - 1, -1, -1):
            self.bubble_down(i)

    def bubble_down(self, vertex):
        left_child_ind = 2 * vertex + 1
        right_child_ind = 2 * vertex + 2
        min_vertex = vertex
        # Check if we're at a leaf
        if left_child_ind >= self.length:
            return
        if self.cities[vertex].weight > self.cities[left_child_ind].weight:
            min_vertex = left_child_ind
        if right_child_ind < self.length and self.cities[min_vertex].weight > self.cities[right_child_ind].weight:
            min_vertex = right_child_ind
        if min_vertex != vertex:
            # Perform a swap
            temp_weight, temp_number = (self.cities[vertex].weight, self.cities[vertex].number)
            self.cities[vertex].weight = self.cities[min_vertex].weight
            self.cities[vertex].number = self.cities[min_vertex].number
            self.cities[min_vertex].weight = temp_weight
            self.cities[min_vertex].number = temp_number
            self.bubble_down(min_vertex)
        return

    def bubble_up(self, vertex):
        if vertex == 0:
            return
        max_vertex = int((vertex - 1) / 2)
        if self.cities[max_vertex].weight > self.cities[vertex].weight:
            temp_weight, temp_number = (self.cities[vertex].weight, self.cities[vertex].number)
            self.cities[vertex].weight = self.cities[max_vertex].weight
            self.cities[vertex].number = self.cities[max_vertex].number
            self.cities[max_vertex].weight = temp_weight
            self.cities[max_vertex].number = temp_number
            self.bubble_up(max_vertex)
        return



class Wedding_Search:
    def __init__(self):
        self.heap = Min_Heap()

    def start(self):
        num_cities = -1
        num_edges = -1
        root_city = -1



if __name__ == '__main__':
    wedding_search = Wedding_Search()
    wedding_search.start()


