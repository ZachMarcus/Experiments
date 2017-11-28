#!/usr/bin/python

import os
import subprocess
import json


def runTest(matrix, tile):
    process = subprocess.check_output(["./matrix-matrix", str(matrix), str(tile)])
    return 1000000 * float(process.replace("\n","")) / (matrix ** 3)


def runTests(matrix_lower_bound, matrix_upper_bound, tile_lower, tile_upper, test_file):
    results = {}
    matrix_sizes = [x for x in range(matrix_lower_bound, matrix_upper_bound, 4)]
    for matrix in matrix_sizes:
        results[matrix] = []
    tile_sizes = [x for x in range(tile_lower, tile_upper, 2)]

    for matrix in matrix_sizes:
        for tile in tile_sizes:
            results[matrix].append(runTest(matrix, tile))

    with open(test_file, 'a+') as result_file:
        all_matrices = sorted(results.keys())
        result_file.write('Tile,')
        for matrix in all_matrices:
            result_file.write(str(matrix))
            if matrix != all_matrices[-1]:
                result_file.write(',')
        result_file.write('\n')

        for tile in range(len(tile_sizes)):
            result_file.write(str(tile_sizes[tile]) + str(','))
            for matrix in all_matrices:
                result_file.write(str(results[matrix][tile]))
                if matrix != all_matrices[-1]:
                    result_file.write(',')
            result_file.write('\n')


def makeCheck():
    if not os.path.exists('./matrix-matrix'):
        process = subprocess.check_output(['make'])
        if process.count('\n') != 1:
            print "Error in build step"
            exit(1)


if __name__ == "__main__":
    makeCheck()
    runTests(20, 100, 1, 20, "results.csv")




