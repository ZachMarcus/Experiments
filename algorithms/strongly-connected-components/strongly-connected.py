#!/usr/bin/python3

import sys
sys.setrecursionlimit(100000)



class City:
    '''
    Class to represent a vertex 
    Contains the number of the city, its outgoing and incoming routes
    '''
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


class Connected_Cities:
    '''
    Major workhorse for this problem
    Runs Kosaraju's strongly connected components algorithm
    Then examines outgoing edges from those components
    To determine all universal roots
    '''
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
#        self.finishing_order.append(city_number)
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

        # Now fix the case where some vertices just don't do anything
        for x in range(1, num_cities + 1):
            if x not in self.cities:
                temp = City(x)
                self.cities[x] = temp

        for x in range(0, num_edges):
            # src, dest, weight
            temp = sys.stdin.readline().rstrip().split(' ') # U, V, D
            src_city = int(temp[0])
            dst_city = int(temp[1])
            self.cities[src_city].outgoing_neighbors[dst_city] = 1
            self.cities[dst_city].incoming_neighbors[src_city] = 1

        for city in self.cities.keys():
            self.predecessors[city] = city
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


        # The alternative to this is to look through the strongly connected
        # components and find if there is a singular component with
        # no outgoing (might be incoming?) edges
        for city_number in self.visited_cities.keys():
            self.visited_cities[city_number] = False
        scc = {} # key is leader, value is outgoing edges
        for city, pred in self.predecessors.items():
            if pred in scc.keys():
                scc[pred].append(city)
            else:
                scc[pred] = [city]

        # Once we have the SCCs in that grouping,
        # We know that: if there is only one SCC without any
        # outgoing connections, then all of those are roots
        # If there are more than that, then there aren't any
        counter = {}
        for leader, cities in scc.items():
            has_outgoing = False
            for city in cities:
                for neighbor in self.cities[city].outgoing_neighbors:
                    if self.predecessors[neighbor] != leader:
                        has_outgoing = True
            if not has_outgoing:
                counter[leader] = len(cities)
        if len(counter.keys()) == 1:
            print(list(counter.values())[0])
        else:
            print(0)
        return


if __name__ == '__main__':
    connected_cities = Connected_Cities()
    connected_cities.start()




