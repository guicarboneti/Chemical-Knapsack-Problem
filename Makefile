CC		=	gcc
CXX		=	g++
DEBUG		+=-Wall
CFLAGS		+=	
CXXFLAGS	+=-std=c++17

all : quimica

quimica : quimica.c
	$(CC) quimica.c $(CFLAGS) -o $@

clean :	
	rm -rf quimica *.o
