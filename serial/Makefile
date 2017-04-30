
CC=gcc
CXX=g++
RM=rm -f

CFLAGS=
CPPFLAGS= -std=c++11
LDFLAGS=
LDLIBS=

SRCS= VecAdd.cpp
OBJS=$(subst .cpp,.o, $(SRCS))

all: VecAdd

VecAdd: $(OBJS)
	$(CXX) $(LDFLAGS) -o VecAdd $(OBJS) $(LDLIBS)

VecAdd.o: VecAdd.cpp # or any other necessary headers or object files here
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -c VecAdd.cpp

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) VecAdd






