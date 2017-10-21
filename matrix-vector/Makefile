

CXX=g++
CFLAGS=-O0 -std=c++11 -fopenmp
LDFLAGS=-fopenmp


matrix-vector: matrix-vector.cpp
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)


all: matrix-vector

clean:
	rm matrix-vector


