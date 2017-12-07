#!/usr/bin/python3

import sys
sys.setrecursionlimit(100000)


class City:
    def __init__(self, number):
        self.number = number
        self.outgoing_neighbors = {} # keys are city nums, vals are weights
        self.incoming_neighbors = {} # keys are city nums, vals are weights
        self.routes_left = 50
        self.reachable_cities = set()

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


class Connected_Cities:
    def __init__(self):
        self.predecessors = {}
        self.visited_cities = {}
        self.cities = {}
        self.finishing_order = [] # list of city.number, first elem finished first
        self.current_traversal_leader = -1
        self.leaders = {}
        self.final_visited_count = {} # k,v = city_number, count visits


    def final_DFS(self, city_number):
        self.visited_cities[city_number] = True # visited
        self.final_visited_count[city_number] = self.final_visited_count[city_number] + 1
        for outgoing_city in self.cities[city_number].outgoing_neighbors.keys():
            if self.visited_cities[outgoing_city] == False: # unvisited
                self.final_DFS(outgoing_city)
        return


    def forward_DFS(self, city_number):
        self.visited_cities[city_number] = True # visited
        self.predecessors[city_number] = self.current_traversal_leader
        for outgoing_city in self.cities[city_number].outgoing_neighbors.keys():
            if self.visited_cities[outgoing_city] == False: # unvisited
                self.forward_DFS(outgoing_city)
        return


    def reversed_DFS(self, city_number):
        self.visited_cities[city_number] = True # visited
        self.predecessors[city_number] = self.current_traversal_leader
        for incoming_city in self.cities[city_number].incoming_neighbors.keys():
            if self.visited_cities[incoming_city] == False: # unvisited
                self.reversed_DFS(incoming_city)
        self.finishing_order.append(city_number)
        return


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
            if self.visited_cities[city] == 0:
                self.current_traversal_leader = city
                self.reversed_DFS(city)

        for city in self.cities.keys():
            self.visited_cities[city] = False

        # Now traverse again, but in forward order and using finishing times
        for x in range(num_cities - 1, -1, -1):
            city_number = self.finishing_order[x]
            self.final_visited_count[city_number] = 0
            if self.visited_cities[city_number] == False: # unvisited
                self.current_traversal_leader = city_number
                self.leaders[city_number] = 0
                # x = last item in the finishing times list
                self.forward_DFS(city_number)

        # For each vertex, leader[v] = vertex from which main loop reached v
        # vertices with the same leader are part of the same SCC

        # NExt up: construct the new graph using just the leaders listed
        # Track how many times a city gets visited
       
        # self.leaders_and_visited_count tracks total number of leaders
        # let's do a DFS from just those and track visits
        # Remember to reset visits
        for leader in self.leaders.keys():
            for city_number in self.visited_cities.keys():
                self.visited_cities[city_number] = False
            self.final_DFS(leader)

        desired_length = len(self.leaders)
        final_count = 0
        
        for city, visit_count in self.final_visited_count.items():
            if visit_count == desired_length:
                final_count += 1
        print(final_count)


if __name__ == '__main__':
    connected_cities = Connected_Cities()
    connected_cities.start()




