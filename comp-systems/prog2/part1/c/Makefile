CC=gcc
ARGS=-c
LIBS=-pthread
EXE=a.out
TEST_EXE=a_test.out
EXE_ARGS=

all:
	$(CC) $(LIBS) -c minsmax.c util.c test.c

clean:
	rm -rf *.o *.gch *.bin $(EXE) $(TEST_EXE)

test: clean	clr
	$(CC) $(LIBS) -o $(TEST_EXE) test.c util.c minsmax.c
	./$(TEST_EXE)

clr:
	clear



