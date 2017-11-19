
CC=gcc
CXX=g++
CFLAGS=-O0 -Wall -std=c11
CXXFLAGS=-O0 -Wall -std=c++11
LDFLAGS=

default: all

matrix-matrix: matrix-matrix.c
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

l3-matrix: l3-matrix.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

matrix-original: matrix-original.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


all: matrix-matrix l3-matrix matrix-original

clean:
	rm matrix-matrix l3-matrix matrix-original


