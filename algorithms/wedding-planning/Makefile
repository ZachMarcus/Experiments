

CXX=g++
CFLAGS=-O3 -std=c++11
LDFLAGS=-fopenmp


wedding-planning: wedding-planning.cpp
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)


all: wedding-planning

clean:
	rm wedding-planning


