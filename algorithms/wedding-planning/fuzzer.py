#!/bin/python3

from random import randint


def generate_examples(num_cities, num_edges, root_city):
    print('{} {} {}'.format(num_cities, num_edges, root_city))
    for x in range(0, num_edges):
        first_city = randint(1, num_cities)
        second_city = randint(1, num_cities)
        while second_city == first_city:
            second_city = randint(1, num_cities)
        weight = randint(1, 23)
        print('{} {} {}'.format(first_city, second_city, weight))



cities = randint(5, 20)
edges = randint(20, 150)
root = randint(1, cities)

generate_examples(cities, edges, root)

