

all: p1 p2 p3 p4

test: p1
	./sieve ${NUM}

p1:
	gcc -g -o sieve.out problem1.c -lm -pthread

p2:
	gcc -g -o p2.out problem2.c -pthread

p3:
	gcc -g -o bank.out problem3.c -pthread

p4:
	gcc -g -o classroom.out problem4.c -pthread


clean:
	rm -rf *.out
