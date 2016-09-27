CC=gcc
ARGS=-c
EXE=memgrind
TEST_EXE=test_malloc

all: memgrind

clean:
	rm -rf *o *.gch memgrind test_malloc

test: test_run

test_run: clean	clr mymalloc.o test_malloc.o
	$(CC) -o $(TEST_EXE) test_malloc.o mymalloc.o
	./test_malloc

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