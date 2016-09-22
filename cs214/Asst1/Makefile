CC=gcc
ARGS=-c
EXE=memgrind

all: memgrind

clean:
	rm -rf *o *.gch memgrind mymalloc

memgrind: mymalloc.o memgrind.o
	$(CC) -o $(EXE) memgrind.o mymalloc.o

memgrind.o:
	$(CC) $(ARGS) memgrind.c
	
mymalloc.o:
	$(CC) $(ARGS) mymalloc.c


	




