#!/usr/bin/python

import os
import subprocess
import json


def runTest(matrix, tile):
    process = subprocess.check_output(["./matrix-matrix", str(matrix), str(tile)])
    return float(process.replace("\n",""))


def runTests(first_bound, second_bound, test_file):
    results = {}
    matrix_sizes = [x * 4 for x in range(first_bound, second_bound)]
    for matrix in matrix_sizes:
        results[matrix] = []
    tile_sizes = []
    n = 1
    while n < first_bound:
        tile_sizes.append(n)
        n += 2
    for matrix in matrix_sizes:
        for tile in tile_sizes:
            results[matrix].append(runTest(matrix, tile))

    with open(test_file, "w") as result_file:
        all_matrices = sorted(results.keys())
        result_file.write("Tile,")
        for matrix in all_matrices:
            result_file.write(str(matrix))
            if matrix != all_matrices[-1]:
                result_file.write(",")
        result_file.write("\n")

        for tile in range(len(tile_sizes)):
            result_file.write(str(tile_sizes[tile]) + str(","))
            for matrix in all_matrices:
                result_file.write(str(results[matrix][tile]))
                if matrix != all_matrices[-1]:
                    result_file.write(",")
            result_file.write("\n")



def makeCheck():
    if not os.path.exists("./matrix-matrix"):
        process = subprocess.check_output(["make"])
        if process.count("\n") != 1:
            print "Error in build step"
            exit(1)


if __name__ == "__main__":
    makeCheck()
    runTests(40, 60, "output.csv")




