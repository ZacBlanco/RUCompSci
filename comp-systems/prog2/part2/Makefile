TEST_EXE=test.out

test3:
	gcc -g mtsort.c mypthread.c queue.c -o ${TEST_EXE} -fno-stack-protector
	./${TEST_EXE}

test2:
	gcc -g mypthread_test.c mypthread.c queue.c -o ${TEST_EXE}
	./${TEST_EXE}

test:
	gcc -g test.c queue.c -o ${TEST_EXE}
	./${TEST_EXE}

run:
	gcc -g mypthread.c queue.c mtsort.c -o ${TEST_EXE}
	gdb -ex=r --args ./${TEST_EXE}

clean:
	rm -rf ${TEST_EXE}

