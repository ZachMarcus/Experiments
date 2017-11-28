

CXX=g++
CFLAGS=-O3 -std=c++11
LDFLAGS=-fopenmp


strongly-connected-components: strongly-connected-components.cpp
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)


all: strongly-connected-components

clean:
	rm strongly-connected-components


