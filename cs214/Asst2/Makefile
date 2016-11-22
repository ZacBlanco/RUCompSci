CC=gcc
ARGS=-c
LIBS=-pthread
EXE=rle.out
TEST_EXE=test_lols.out
EXE_ARGS=

all: lols

clean:
	rm -rf *txt_LOLS* *.o *.gch *.bin $(EXE) $(TEST_EXE)

test: test_run

debug: gdb_lols

test_run: clean	clr compressR_LOLS.o compressT_LOLS.o compressR_worker_LOLS.c test_lols.o compressR_worker_LOLS.o
	$(CC) $(LIBS) -o $(TEST_EXE) lols.o compressR_LOLS.o compressT_LOLS.o test_lols.o
	./$(TEST_EXE)

clr:
	clear

lols: clean	clr compressR_LOLS.o compressT_LOLS.o compressR_worker_LOLS.c test_lols.o compressR_worker_LOLS.o
	$(CC) $(LIBS) -o $(TEST_EXE) lols.o compressR_LOLS.o compressT_LOLS.o test_lols.o


compressT_LOLS.o: lols.o
	$(CC) $(ARGS) compressT_LOLS.c

compressR_LOLS.o: lols.o
	$(CC) $(ARGS) compressR_LOLS.c

compressR_worker_LOLS.o: lols.o
	$(CC) -o comp_proc.bin  compressR_worker_LOLS.c lols.o

lols.o:
	$(CC) $(ARGS) lols.c

test_lols.o:
	$(CC) $(ARGS) test_lols.c

gdb_lols: gdb_test_lols.o
	$(CC) -g $(LIBS) $(EXE_ARGS) -o $(EXE) lols.o test_lols.o compressT_LOLS.o compressR_LOLS.o 

gdb_compressT_LOLS.o: gdb_lols.o
	$(CC) -g $(ARGS) compressT_LOLS.c

gdb_compressR_LOLS.o: gdb_lols.o
	$(CC) -g $(ARGS) compressR_LOLS.c

gdb_compressR_worker_LOLS.o: gdb_lols.o
	$(CC) -g $(ARGS) compressR_worker_LOLS.c

gdb_test_lols.o: gdb_compressT_LOLS.o gdb_compressR_worker_LOLS.o gdb_compressR_LOLS.o
	$(CC) -g $(ARGS) test_lols.c

gdb_lols.o:
	$(CC) -g $(ARGS) lols.c

