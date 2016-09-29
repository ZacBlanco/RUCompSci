CC=gcc
ARGS=-c

clean:
	rm -rf *.o main.out
clear:
	clear
all: main

main: part1_main.o part1_functions.o
	$(CC) -o main.out part1_main.o part1_functions.o 
test: clean main
	clear
	./main.out

part1_main.o: part1_functions.o
	$(CC) $(ARGS) part1_main.c

part1_functions.o:
	$(CC) $(ARGS) part1_functions.c
	
