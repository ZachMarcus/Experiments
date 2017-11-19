# Matrix Inversion

### Building: 
Only load the compiler module or similar one where necessary.
This particular module is available on the Northeastern Discovery Cluster.
```
module load mpich-3.0.4
module load gnu-4.9.3-compilers
make


### Usage:

Remember to have an appropriate hostfile for your host(s)
```
mpirun -np [#Processes] -hostfile hostfile ./monte-carlo [numIterations]
```
