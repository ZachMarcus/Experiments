

CXX=g++
CFLAGS=-O3 -std=c++11 -fopenmp
LDFLAGS=-fopenmp

default: matrix-inversion

matrix-inversion: matrix-inversion.cpp
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

all: matrix-inversion

clean:
	rm -f matrix-inversion


