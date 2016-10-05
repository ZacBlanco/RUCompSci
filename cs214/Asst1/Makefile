CC=gcc
ARGS=-c
EXE=memgrind.out
TEST_EXE=test_malloc.out
EXE_ARGS=

all: memgrind

clean:
	rm -rf *o *.gch memgrind.out test_malloc.out

test: test_run

debug: gdb_memgrind

test_run: clean	clr mymalloc.o test_malloc.o
	$(CC) -o $(TEST_EXE) test_malloc.o mymalloc.o
	./test_malloc.out

clr:
	clear

memgrind: clean mymalloc.o memgrind.o
	$(CC) $(EXE_ARGS) -o $(EXE) memgrind.o mymalloc.o

memgrind.o:
	$(CC) $(ARGS) memgrind.c
	
mymalloc.o:
	$(CC) $(ARGS) mymalloc.c

test_malloc.o:
	$(CC) $(ARGS) test_malloc.c


gdb_memgrind: clean gdb_mymalloc.o gdb_memgrind.o
	$(CC) -g $(EXE_ARGS) -o $(EXE) memgrind.o mymalloc.o

gdb_memgrind.o:
	$(CC) -g $(ARGS) memgrind.c
	
gdb_mymalloc.o:
	$(CC) -g $(ARGS) mymalloc.c
