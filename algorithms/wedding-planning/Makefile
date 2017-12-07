

CXX=g++
CFLAGS=-O3 -std=c++11
LDFLAGS=-fopenmp


wedding-planner: wedding-planner.cpp
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)


all: wedding-planner

clean:
	rm wedding-planner


