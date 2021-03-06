#!/usr/bin/python3

import sys


class City:
    def __init__(self, number, weight):
        self.number = number
        self.weight = weight

    def __eq__(self, rhs):
        return not self.weight < rhs.weight and not rhs.weight < self.weight
    def __ne__(self, rhs):
        return self.weight < rhs.weight or rhs.weight < self.weight
    def __gt__(self, rhs):
        return rhs.weight < self.weight
    def __lt__(self, rhs):
        return self.weight < rhs.weight
    def __ge__(self, rhs):
        return not self.weight < rhs.weight
    def __le__(self, rhs):
        return not rhs.weight < self.weight


class Min_Heap:
    def __init__(self):
        self.length = 0
        self.cities = []
        self.cities_mapping = {}

    def swap(self, left_index, right_index):
        # swap values
        temp_weight = self.cities[left_index].weight
        temp_number = self.cities[left_index].number
        self.cities[left_index].weight = self.cities[right_index].weight
        self.cities[left_index].number = self.cities[right_index].number
        self.cities[right_index].weight = temp_weight
        self.cities[right_index].number = temp_number
        # Swap the keys
        number_of_left = self.cities[left_index].number
        number_of_right = self.cities[right_index].number
        temp_mapping = self.cities_mapping[number_of_left]
        self.cities_mapping[number_of_left] = self.cities_mapping[number_of_right]
        self.cities_mapping[number_of_right] = temp_mapping
        return


    def update(self, city_number, new_weight):
        index = self.cities_mapping[city_number]
        self.cities[index].weight = new_weight
        self.bubble_up(index)
        return
        for index in range(0, len(self.cities)):
            if self.cities[index].number == city_number:
                self.cities[index].weight = new_weight
                self.bubble_up(index)
                return
        
    def insert(self, city):
        self.cities.append(city)
        self.cities_mapping[city.number] = self.length
        self.bubble_up(self.length)
        self.length += 1
        return

    def top(self):
        if self.length == 0:
            return
        return self.cities[0]

    def pop(self):
        if self.length == 0:
            return
        self.length -= 1
        temp_city = self.cities[0]
        del self.cities_mapping[temp_city.number]
        self.cities[0] = self.cities[self.length]
        self.cities_mapping[self.cities[0].number] = 0
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
            self.swap(min_vertex, vertex)
            self.bubble_down(min_vertex)
        return

    def bubble_up(self, vertex):
        if vertex == 0:
            return
        max_vertex = int((vertex - 1) / 2)
        if self.cities[max_vertex].weight > self.cities[vertex].weight:
            self.swap(max_vertex, vertex)
            self.bubble_up(max_vertex)
        return


class Wedding_Planner:
    def __init__(self):
        self.visited_cities = {}
        self.distances = {}
        self.outgoing_vertices = {}
        self.min_heap = Min_Heap()

    def plan(self):
        num_cities = -1
        num_edges = -1
        root_city = -1
        first_line = sys.stdin.readline().rstrip().split(' ')
        num_cities = int(first_line[0])
        num_edges = int(first_line[1])
        root_city = int(first_line[2])
        
        for vertex in range(1, num_cities+1):
            self.outgoing_vertices[vertex] = {}

        for x in range(0, num_edges):
            # src, dest, weight
            temp = sys.stdin.readline().rstrip().split(' ') # U, V, D
            src_city = int(temp[0])
            dst_city = int(temp[1])
            path_weight = int(temp[2])        
            self.outgoing_vertices[dst_city][src_city] = path_weight

        # intialization
        for city in range(1, num_cities + 1):
            self.visited_cities[city] = False
            self.distances[city] = float('inf')
            self.min_heap.insert(City(city, float('inf')))

        self.distances[root_city] = 0
        self.min_heap.update(root_city, 0)

        while not self.min_heap.is_empty():
            city = self.min_heap.top()
            self.min_heap.pop()
            self.visited_cities[city.number] = True
            for vertex, weight in self.outgoing_vertices[city.number].items():
                new_dist = self.distances[city.number] + weight
                old_dist = self.distances[vertex]
                if new_dist < old_dist:
                    self.distances[vertex] = new_dist
                    self.min_heap.update(vertex, new_dist)

        ret = ''
        for city in range(1, num_cities+1):
            if self.distances[city] == float('inf'):
                ret += '-1 '
            else:
                ret += str(self.distances[city]) + ' '
        print(ret[:-1])





if __name__ == '__main__':
    wedding_planner = Wedding_Planner()
    wedding_planner.plan()

