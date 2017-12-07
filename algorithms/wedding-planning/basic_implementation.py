#!/usr/bin/python3

import collections
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

    def update(self, city_number, new_weight):
        for index in range(0, len(self.cities)):
            if self.cities[index].number == city_number:
                self.cities[index].weight = new_weight
                self.bubble_up(index)
                return
        
    def insert(self, city):
        self.cities.append(city)
        self.bubble_up(self.length)
        self.length += 1
        return

    def top(self):
        if self.length == 0:
            print('Error, cities is empty')
            return
        return self.cities[0]

    def pop(self):
        if self.length == 0:
            return
        self.length -= 1
        temp_city = self.cities[0]
        self.cities[0] = self.cities[self.length]
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




class Heap_Map(collections.MutableMapping):
    def __init__(self):
        self.heap = []
        self.dic = dict()
        self.update()

    @staticmethod
    def parent(i):
        return ((i - 1) >> 1)

    @staticmethod
    def right(i):
        return ((i + 1) << 1)

    @staticmethod
    def left(i):
        return ((i << 1) + 1)

    def clear(self):
        del self.heap[:]
        self.dic.clear()

    def __setitem__(self, key, value):
        if key in self.dic:
            self.pop(key)
        temp = []
        temp.append(value)
        temp.append(key)
        temp.append(len(self))
        self.dic[key] = temp
        self.heap.append(temp)
        self.bubble_down(len(self.heap) - 1)

    def heapify(self, i):
        left = self.left(i)
        right = self.right(i)
        elements = len(self.heap)
        if left < elements and self.heap[left][0] < self.heap[i][0]:
            lower_bound = left
        else:
            lower_bound = i
        if right < elements and self.heap[right][0] < self.heap[lower_bound][0]:
            lower_bound = right

        if lower_bound != i:
            self.swap(i, lower_bound)
            self.heapify(lower_bound)

    def bubble_down(self, key):
        while key:
            temp = self.parent(key)
            if self.heap[temp][0] < self.heap[key][0]:
                return
            self.swap(key, temp)
            key = temp

    def swap(self, lhs, rhs):
        temp = self.heap[lhs]
        self.heap[lhs] = self.heap[rhs]
        self.heap[rhs] = temp
        self.heap[lhs][2] = lhs
        self.heap[rhs][2] = rhs

    def __delitem__(self, key):
        temp = self.dic[key]
        while temp[2]:
            temp_parent = self.parent(temp[2])
            parent = self.heap[temp_parent]
            self.swap(temp[2], parent[2])
        self.pop2()

    def __getitem__(self, key):
        return self.dic[key][0]

    def __iter__(self):
        return iter(self.dic)

    def pop2(self):
        temp = self.heap[0]
        if len(self.heap) == 1:
            self.heap.pop()
        else:
            self.heap[0] = self.heap.pop(-1)
            self.heap[0][2] = 0
            self.heapify(0)
        del self.dic[temp[1]]
        return temp[0]

    def __len__(self):
        return len(self.dic)

    def top(self):
        return self.heap[0][0]




class Wedding_Planner:
    def __init__(self):
        self.visited_cities = {}
        self.distances = {}
        self.outgoing_vertices = {}
#        self.heap_map = Heap_Map()
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
#            self.heap_map[city] = City(city, float('inf'))
            self.min_heap.insert(City(city, float('inf')))

        self.distances[root_city] = 0
#        self.heap_map[root_city] = City(root_city, 0)
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

        '''while len(self.heap_map) > 0:
            min_city = self.heap_map.top()
            self.heap_map.pop2()
            self.visited_cities[min_city.number] = True
            for vertex, weight in self.outgoing_vertices[min_city.number].items():
                new_dist = self.distances[min_city.number] + weight
                old_dist = self.distances[vertex]
                if new_dist < old_dist:
                    self.distances[vertex] = new_dist
                    self.heap_map[vertex] = City(vertex, new_dist)'''


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







