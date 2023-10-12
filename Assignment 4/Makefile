#
# example Makefile to build an executable named myprog from myprog.cpp
#
PROG=performance

all: $(PROG).c
	mpicc $(PROG).c -lm -Wall -o $(PROG)

clean:
	$(RM) $(PROG)
