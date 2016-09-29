CC=gcc
ARGS=-c
EXE=memgrind.out
TEST_EXE=test_malloc.out

all: memgrind

clean:
	rm -rf *o *.gch memgrind.out test_malloc.out

test: test_run

test_run: clean	clr mymalloc.o test_malloc.o
	$(CC) -o $(TEST_EXE) test_malloc.o mymalloc.o
	./test_malloc.out

clr:
	clear

memgrind: mymalloc.o memgrind.o
	$(CC) -o $(EXE) memgrind.o mymalloc.o

memgrind.o:
	$(CC) $(ARGS) memgrind.c
	
mymalloc.o:
	$(CC) $(ARGS) mymalloc.c

test_malloc.o:
	$(CC) $(ARGS) test_malloc.c
