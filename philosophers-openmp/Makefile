

CXX=g++
CFLAGS=-O3 -std=c++11
LDFLAGS=-fopenmp


philosophers: philosophers.cpp
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)


all: philosophers

clean:
	rm philosophers


