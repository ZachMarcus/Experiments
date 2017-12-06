#!/usr/bin/python3

import sys
sys.setrecursionlimit(100000)


class City:
    def __init__(self, number):
        self.number = number
        self.outgoing_neighbors = {} # keys are city nums, vals are weights
        self.incoming_neighbors = {} # keys are city nums, vals are weights
        self.routes_left = 50

    def __str__(self):
        ret = ''
        ret += 'City Number: ' + str(self.number)
        ret += '\nOutgoing(): '
        for key, val in self.outgoing_neighbors.items():
            ret += '(' + str(key) + ',' + str(val) + ');'
        ret += '\nIncoming(): '
        for key, val in self.incoming_neighbors.items():
            ret += '(' + str(key) + ',' + str(val) + ');'
        return ret

class Stack:
    def __init__(self):
        self.items = []
    def is_empty(self):
        return self.items == []
    def push(self, item):
        self.items.append(item)
    def pop(self):
        return self.items.pop()
    def peek(self):
        return self.items[-1]
    def size(self):
        return len(self.items)


class Connected_Cities:
    def __init__(self):
        self.stack = Stack()
        self.predecessors = {}
        self.visited_cities = {}
        self.cities = {}
        self.finishing_order = [] # list of city.number, first elem finished first
        self.current_traversal_leader = -1

    def reversed_DFS(self, city_number):
        self.visited_cities[city_number] = True
        self.predecessors[city_number] = self.current_traversal_leader
        for outgoing_city in self.cities[city_number].outgoing_neighbors.keys():
            if self.visited_cities[outgoing_city] == False:
                self.reversed_DFS(outgoing_city)
        self.finishing_order.append(city_number)


    def start(self):
        num_cities = -1
        num_edges = -1
        first_line = sys.stdin.readline().rstrip().split(' ')
        num_cities = int(first_line[0])
        num_edges = int(first_line[1])
        for x in range(0, num_edges):
            # src, dest, weight
            temp = sys.stdin.readline().rstrip().split(' ') # U, V, D
            src_city = int(temp[0])
            dst_city = int(temp[1])

            if src_city not in self.cities:
                city = City(src_city)
                city.outgoing_neighbors[dst_city] = 1
                self.cities[src_city] = city
            else: # Add value to outgoing neighbors adjacency list
                city = self.cities[src_city]
                city.outgoing_neighbors[dst_city] = 1
                self.cities[src_city] = city
            if dst_city not in self.cities:
                city = City(dst_city)
                city.incoming_neighbors[src_city] = 1
                self.cities[dst_city] = city
            else: # Add value to incoming neighbors adjacency list
                city = self.cities[dst_city]
                city.incoming_neighbors[src_city] = 1
                self.cities[dst_city] = city
        # Now fix the case where some vertices just don't do anything
        for x in range(1, num_cities + 1):
            if x not in self.cities:
                temp = City(x)
                self.cities[x] = temp

        for city in self.cities.keys():
            self.predecessors[city] = city

        # Keep visited dictionary, we've visited 0 cities
        for city in self.cities.keys():
            self.visited_cities[city] = False

        # For each node, if unvisited, do self.reversed_DFS(city_number) on graph starting from there
        for city in self.cities.keys():
            if self.visited_cities[city] == False:
                self.current_traversal_leader = city
                self.reversed_DFS(city)

        for x in self.finishing_order:
            print('{} Finished'.format(x))

        for x in self.predecessors.keys():
            print('leader[{}]={}'.format(x, self.predecessors[x]))

        for city in self.cities.keys():
            self.visited_cities[city] = False

        # Now traverse again, but in forward order and using finishing times
        for x in range(num_cities - 1, -1, -1):
            city_number = self.finishing_order[x]
            if self.visited_cities[city_number] == False:
                self.current_traversal_leader = city_number
                # x = last item in the finishing times list
                self.forward_DFS(city_number)


            # Using the finishing times, run DFS_main on G, not G rev
            # Process vertices in decreasing order of finishing times
            # For each vertex, leader[v] = vertex from which main loop reached v



            '''
            for incoming, weight in city.incoming_neighbors.items():
                curr_dist = self.distances[incoming]
                other_dist = self.distances[city.number] + weight
                
                if curr_dist > other_dist:
                    self.distances[incoming] = other_dist
                    self.predecessors[incoming] = city.number
                    # Now we need to update the Heap
                    if incoming in self.heap:
                        self.heap[incoming].weight = other_dist
#                    else:
#                        print('{} Not in heap anymore'.format(incoming))
#            print('Distances: {}\nPredecessors: {}'.format(self.distances, self.predecessors))'''
            
#        print('Distances: {}\nPredecessors: {}'.format(self.distances, self.predecessors))
        to_print = ''
        for x in self.predecessors.keys():
            to_print += str(self.predecessors[x]) + ' '
        to_print = to_print[:-1]
        print(to_print)


if __name__ == '__main__':
    connected_cities = Connected_Cities()
    connected_cities.start()






