

CXX=gcc
CFLAGS=-O0 -Wall -std=c11
LDFLAGS=

default: all

matrix-matrix: matrix-matrix.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

l3-matrix: l3-matrix.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


all: matrix-matrix l3-matrix

clean:
	rm matrix-matrix l3-matrix


