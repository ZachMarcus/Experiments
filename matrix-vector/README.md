#Matrix-Vector Multiplication

###Building: 
```
make
```
Note that to print the resulting vector output, you must uncomment the "#define PRINT" line


###Usage:
```
./matrix-vector [numElements] [doSerial] [doOpenMP]
```

The performance numbers can be found at perf.csv and a graph depicting those is also present in this directory.

##Performance
![Performance Chart](https://github.com/ZachMarcus/Experiments/raw/master/matrix-vector/matrix-vector-perf.png)


The OpenMP version leverages the OpenMP built-in reduction to compute each element of the output vector, but older versions of OpenMP do no support doing this directly onto an array element so the result is reduced on a temporary array before being copied to the vector.

