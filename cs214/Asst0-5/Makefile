CC=gcc
args:-o

clean:
	rm -rf *.o main

all: part1_main.o

part1_main.o: part1_func.o
	$(CC) $(args) part1_main.c

part1_func.c:
	$(CC) $(args) part1_functions.c
	
