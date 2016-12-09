CC=gcc
ARGS=-c
LIBS=-pthread
EXE=libnet.out
TEST_EXE=test.out
EXE_ARGS=

all: libnet

clean:
	rm -rf  *.o *.gch *.bin $(EXE) $(TEST_EXE)

test: test_run

test_run: clean	clr libnet
	$(CC) $(LIBS) -o $(TEST_EXE) netfileserver.o libnetfiles.o
	./$(TEST_EXE)

clr:
	clear

libnet: clr clr netfileserver.o libnetfiles.o
	$(CC) $(LIBS) -o $(TEST_EXE) netfileserver.o libnetfiles.o


libnetserver.o: libnetfiles.o
	$(CC) $(ARGS) netfileserver.c

libnetfiles.o:
	$(CC) $(ARGS) libnetfiles.c

