#!/usr/bin/python3

import sys



class City:
    def __init__(self, number, weight):
        self.number = number
        self.weight = weight
        self.outgoing_neighbors = {} # keys are city nums, vals are weights
        self.incoming_neighbors = {} # keys are city nums, vals are weights

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
        ret += '\nOutgoing(#,Weight): '
        for key, val in self.outgoing_neighbors.items():
            ret += '(' + str(key) + ',' + str(val) + ');'
        ret += '\nIncoming(#,Weight): '
        for key, val in self.incoming_neighbors.items():
            ret += '(' + str(key) + ',' + str(val) + ');'
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
        temp_heap = self.min_heap
        while temp_heap[0][1] not in self or self[temp_heap[0][1]] != temp_heap[0][0]:
            last_heap_item = temp_heap.pop()
            where_to_put_it = 0
            running = True
            while running:
                child_index = 2 * where_to_put_it + 1
                if child_index + 1 < len(temp_heap) and temp_heap[child_index] > temp_heap[child_index + 1]:
                    child_index += 1
                if child_index >= len(temp_heap) or last_heap_item <= temp_heap[child_index]:
                    temp_heap[where_to_put_it] = last_heap_item
                    running = False
                if running:
                    temp_heap[where_to_put_it] = temp_heap[child_index]
                    where_to_put_it = child_index
        return temp_heap[0][1]

    def __iter__(self):
        '''
        Destroy elements as it iterates
        '''
        def iteration_lambda():
            while len(self) > 0:
                temp = self.pop()
                yield temp
                del self[temp]
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


class Wedding_Search:
    def __init__(self):
        self.heap = Heap_Map()
        self.distances = []
        self.predecessors = []
        self.visited_cities = []

    def start(self):
        num_cities = -1
        num_edges = -1
        root_city = -1
        first_line = sys.stdin.readline().rstrip().split(' ')
        num_cities = int(first_line[0])
        num_edges = int(first_line[1])
        root_city = int(first_line[2])
        for x in range(0, num_edges):
            # src, dest, weight
            temp = sys.stdin.readline().rstrip().split(' ') # U, V, D
            src_city = int(temp[0])
            dst_city = int(temp[1])
            path_weight = int(temp[2])
            if src_city not in self.heap:
                city = City(src_city, float('inf'))
                city.outgoing_neighbors[dst_city] = path_weight
                self.heap[src_city] = city
            else: # Add value to outgoing neighbors adjacency list
                city = self.heap[src_city]
                city.outgoing_neighbors[dst_city] = path_weight
                self.heap[src_city] = city
            if dst_city not in self.heap:
                city = City(dst_city, float('inf'))
                city.incoming_neighbors[src_city] = path_weight
                self.heap[dst_city] = city
            else: # Add value to incoming neighbors adjacency list
                city = self.heap[dst_city]
                city.incoming_neighbors[src_city] = path_weight
                self.heap[dst_city] = city
        # Now fix the root city:
        root = self.heap[root_city]
        root.weight = 0
        self.heap[root_city] = root

        for key, val in self.heap.items():
            print('Key: {}, Val: {}'.format(key, val))

        self.distances.append(None) # The cities are 1-indexed
        self.predecessors.append(None) # The predecessors are 1-indexed
        for city in self.heap.keys():
            self.distances.append(float('inf'))
            self.predecessors.append(None)

        print('Distances: {}\nPredecessors: {}'.format(self.distances, self.predecessors))

        while len(self.heap) > 0:
            city = self.heap.pop() # Extract-Min
            print(city)
            print(type(city))
            self.visited_cities.append(city) # S := S Union with new City
            for outgoing, weight in city.outgoing_neighbors.items():
                curr_dist = self.distances[city.number]
                other_dist = self.distances[outgoing] + weight
                if curr_dist > other_dist:
                    self.distances[outgoing] = other_dist
                    self.predecessors[outgoing] = city.number
                    # Now we need to update the Heap
                    self.heap[outgoing].weight = other_dist
            
        print('Distances: {}\nPredecessors: {}'.format(self.distances, self.predecessors))



if __name__ == '__main__':
    wedding_search = Wedding_Search()
    wedding_search.start()


