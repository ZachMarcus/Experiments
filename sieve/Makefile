

CXX=g++
CFLAGS=-O3 -std=c++11
LDFLAGS=-pthread


sieve: sieve.cpp
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)


all: sieve

clean:
	rm sieve


