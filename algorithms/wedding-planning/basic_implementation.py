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

    def __str__(self):
        ret = ''
        ret += 'City Number: ' + str(self.number) + ' Weight: ' + str(self.weight)
        return ret





# Implements classic heap functionality in that you can get the smallest items
# first, but also has good access times as a python dictionary
class Heap_Map(dict):
    def __init__(self):
        self.min_heap = []
        dict.__init__(self)

    def pop(self):
        if len(self) == 0:
            print('Heap_dictionary is empty')
#        temp_heap = self.min_heap
        while self.min_heap[0][1] not in self or self[self.min_heap[0][1]] != self.min_heap[0][0]:
            last_heap_item = self.min_heap.pop()
            where_to_put_it = 0
            running = True
            while running:
                child_index = 2 * where_to_put_it + 1
                if child_index + 1 < len(self.min_heap) and self.min_heap[child_index] > self.min_heap[child_index + 1]:
                    child_index += 1
                if child_index >= len(self.min_heap) or last_heap_item <= self.min_heap[child_index]:
                    self.min_heap[where_to_put_it] = last_heap_item
                    running = False
                if running:
                    self.min_heap[where_to_put_it] = self.min_heap[child_index]
                    where_to_put_it = child_index
        return self.min_heap[0][0]

    def __iter__(self):
        '''
        Destroy elements as it iterates
        For use in main Dijkstra algorithm
        '''
        def iteration_lambda():
            while len(self) > 0:
                temp = self.pop()
                yield temp
                del self[temp.number]
        return iteration_lambda()

    def __setitem__(self, key, val):
        dict.__setitem__(self, key, val)
        temp_heap = self.min_heap
        if len(temp_heap) > 2 * len(self):
            self.min_heap = [(value, key) for key, value in self.items()]
            self.min_heap.sort()
        else:
            temp = (val, key)
            where_to_put_it = len(temp_heap)
            temp_heap.append(None)
            while where_to_put_it > 0 and temp < temp_heap[(where_to_put_it - 1) // 2]:
                temp_heap[where_to_put_it] = temp_heap[(where_to_put_it - 1) // 2]
                where_to_put_it = (where_to_put_it - 1) // 2
            temp_heap[where_to_put_it] = temp

    def setdefault(self, key, val):
        '''
        A new version of setdefault
        '''
        if key not in self:
            self[key] = val
        return self[key]









class Wedding_Planner:
    def __init__(self):
        self.visited_cities = {}
        self.distances = {}
        self.outgoing_vertices = {}
        self.heap_map = Heap_Map()

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

        self.distances[root_city] = 0

        # Actual algorithm
        for city in range(1, num_cities + 1):
            # Find unvisited vertex with minimum distances
            minimum_vertex_val = float('inf')
            minimum_vertex = -1
            for city in range(1, num_cities + 1):
                if self.visited_cities[city] == False and self.distances[city] < minimum_vertex_val:
                    minimum_vertex = city
                    minimum_vertex_val = self.distances[city]
            if minimum_vertex == -1:
                for city in range(1, num_cities + 1):
                    if self.visited_cities[city] == False:
                        minimum_vertex = city
                        break
            # REPLACE the above with the heap later.
            # Now visit the found minimum vertex
            self.visited_cities[minimum_vertex] = True
            # And relax the adjacent edges to update the shortest path distance to neighbors
            for vertex, weight in self.outgoing_vertices[minimum_vertex].items():
                new_dist = self.distances[minimum_vertex] + weight
                old_dist = self.distances[vertex]
                if new_dist < old_dist:
                    self.distances[vertex] = new_dist

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







