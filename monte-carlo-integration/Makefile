

CXX=g++
CFLAGS=-O3 -Wall
LDFLAGS= -fopenmpi


monte-carlo: monte-carlo.cpp
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)


all: monte-carlo

clean:
	rm monte-carlo


