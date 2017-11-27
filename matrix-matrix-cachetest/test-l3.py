#!/usr/bin/python

import os
import subprocess
import json


def runTest(matrix, tile):
    process = subprocess.check_output(["./matrix-matrix", str(matrix), str(tile)])
    return float(process.replace("\n",""))

def run_l3(matrix_size):
    process = subprocess.check_output(["./l3-matrix", str(matrix_size)])
    return float(process.replace("\n",""))


def runTests(first_bound, second_bound, test_file):
    results = {}
    matrix_sizes = [x for x in range(first_bound, second_bound)]
    for matrix in matrix_sizes:
        results[matrix] = 0.
    for matrix in matrix_sizes:
        results[matrix] = run_l3(matrix)

    with open(test_file, "a+") as result_file:
        all_matrices = sorted(results.keys())
        result_file.write("Size,Speed\n")
        for matrix in all_matrices:
            result_file.write("{},{}\n".format(matrix, results[matrix]))


def makeCheck():
    if not os.path.exists("./matrix-matrix"):
        process = subprocess.check_output(["make"])
        if process.count("\n") != 1:
            print "Error in build step"
            exit(1)


if __name__ == "__main__":
    makeCheck()
    runTests(4, 600, "output.csv")




