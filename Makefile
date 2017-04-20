# This is comment 1
SHELL = /bin/sh
FLAGS = -g
CC = g++

all:    p1 p2 p3  

p1:     TwoPipesTwoChildren.cpp
	$(CC) $(FLAGS) -o $@ TwoPipesTwoChildren.cpp

p2:     TwoPipesThreeChildren.cpp
	$(CC) $(FLAGS) -o $@ TwoPipesThreeChildren.cpp

p3:	DynPipe.cpp
	$(CC) $(FLAGS) -o $@ DynPipe.cpp
clean:
	rm p1 p2 p3
