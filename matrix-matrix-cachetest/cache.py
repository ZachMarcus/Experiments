#!/usr/bin/python3

import subprocess


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

