

CXX=g++
CFLAGS=-O0 -std=c++11 -fopenmp
LDFLAGS=-fopenmp


matrix-inversion: matrix-inversion.cpp
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)


all: matrix-inversion

clean:
	rm matrix-inversion


