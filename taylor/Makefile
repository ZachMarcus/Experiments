

CXX=g++
CFLAGS=-O3 -std=c++11
LDFLAGS=-pthread


taylor: taylor.cpp
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)


all: taylor

clean:
	rm taylor


