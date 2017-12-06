#!/usr/bin/python3

import sys




class Wedding_Planner:
    def __init__(self):
        self.visited_cities = {}
        self.distances = {}
        self.outgoing_vertices = {}


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







