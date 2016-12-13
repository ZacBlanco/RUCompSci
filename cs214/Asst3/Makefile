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
	$(CC) $(LIBS) -o $(TEST_EXE) libnetfiles.o test_files.o
	./$(TEST_EXE)

clr:
	clear

server:  libnetfiles.o libnetserver.o 
	$(CC) $(LIBS) -o server.out netfileserver.o libnetfiles.o
	./server.out

libnet: clr clr netfileserver.o libnetfiles.o test_files.o
	$(CC) $(LIBS) -o $(TEST_EXE) libnetfiles.o test_files.o


libnetserver.o: libnetfiles.o
	$(CC) $(ARGS) netfileserver.c

libnetfiles.o:
	$(CC) $(ARGS) libnetfiles.c

test_files.o:
	$(CC) $(ARGS) test_files.c
