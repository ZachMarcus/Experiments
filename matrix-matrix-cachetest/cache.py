#!/usr/bin/python3

import subprocess
import math

base_command = "cat /sys/devices/system/cpu/cpu0/cache/index"
get_L1D_size = base_command + "0/size"
get_L1I_size = base_command + "1/size"
get_L2_size = base_command + "2/size"
get_L3_size = base_command + "3/size"


L1 = subprocess.check_output(get_L1D_size.split(" ")).decode("utf8").rstrip()
L2 = subprocess.check_output(get_L2_size.split(" ")).decode("utf8").rstrip()
L3 = subprocess.check_output(get_L3_size.split(" ")).decode("utf8").rstrip()


print("L1: {}".format(L1))
print("L2: {}".format(L2))
print("L3: {}".format(L3))



if "K" in L1 and "K" in L2 and "K" in L3:
    L1 = int(L1[:-1]) * 1000
    L2 = int(L2[:-1]) * 1000
    L3 = int(L3[:-1]) * 1000
else:
    exit()

size_of_int = 4 # bytes


L1_elements = int(L1 / size_of_int)
L2_elements = int(L2 / size_of_int)
L3_elements = int(L3 / size_of_int)

print("{} int elements allowed in L1".format(L1_elements))
print("{} int elements allowed in L2".format(L2_elements))
print("{} int elements allowed in L3".format(L3_elements))


L1_matrix_size = math.sqrt(L1_elements)
L2_matrix_size = math.sqrt(L2_elements)
L3_matrix_size = math.sqrt(L3_elements)

print("L1 matrix size: {}".format(L1_matrix_size))
print("L2 matrix size: {}".format(L2_matrix_size))
print("L3 matrix size: {}".format(L3_matrix_size))





