#!/bin/bash

# To get available nodes to allocate
#sinfo | grep idle

# To allocate one such node
salloc -N 1 --exclusive -p par-gpu

# To list the allocated node
#squeue -l -u marcus.z

# To login to the node
#ssh compute-2-184
