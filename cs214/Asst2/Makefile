CC=gcc
ARGS=-c -pthread
EXE=rle.out
TEST_EXE=test_rle.out
EXE_ARGS=

all: rle

clean:
	rm -rf *.o *.gch $(EXE) $(TEST_EXE)

test: test_run

debug: gdb_rle

test_run: clean	clr compressR_LOLS.o compressT_LOLS.o compressR_worker_LOLS.c test_rle.o
	$(CC) -o $(TEST_EXE) compressR_LOLS.o compressT_LOLS.o test_rle.o
	./$(TEST_EXE)

clr:
	clear

compressT_LOLS.o:
	$(CC) $(ARGS) compressT_LOLS.c

compressR_LOLS.o:
	$(CC) $(ARGS) compressR_LOLS.c

compressR_worker_LOLS.o:
	$(CC) $(ARGS) compressR_worker_LOLS.c

test_rle.o:
	$(CC) $(ARGS) test_rle.c

gdb_rle: gdb_test_rle.o
	$(CC) -g $(EXE_ARGS) -o $(EXE) test_rle.o compressT_LOLS.o compressR_worker_LOLS.o compressR_LOLS.o 

gdb_compressT_LOLS.o:
	$(CC) -g $(ARGS) compressT_LOLS.c

gdb_compressR_LOLS.o:
	$(CC) -g $(ARGS) compressR_LOLS.c

gdb_compressR_worker_LOLS.o:
	$(CC) -g $(ARGS) compressR_worker_LOLS.c

gdb_test_rle.o: gdb_compressT_LOLS.o gdb_compressR_worker_LOLS.o gdb_compressR_LOLS.o
	$(CC) -g $(ARGS) test_rle.c

