CC		=	gcc
CXX		=	g++
DEBUG		+=-Wall
CFLAGS		+=-O2	
CXXFLAGS	+=-O2 -std=c++17

all : quimica

quimica : quimica.c
	$(CC) quimica.c $(CFLAGS) -o $@

quimica_teste : quimica_teste.cpp
	$(CXX) quimica_teste.cpp $(CXXFLAGS) -o $@

semBound : semBound.c
	$(CC) semBound.c $(CFLAGS) -o $@

clean :	
	rm -rf quimica semBound quimica_teste *.o
