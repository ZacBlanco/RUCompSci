#define _POSIX_SOURCE
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int res = 0, num = 2, pd[2], arg[32];
pthread_t thread[32];
sem_t sem;

void h(int s)
{
	read(pd[0], &num, sizeof(num));
	sem_post(&sem);
	printf("h\n");
}

void *f(void *var)
{

	int tmp = *(int *) var;
	res += tmp;
	pthread_exit((void *)0);
	printf("f\n");
}

int main()
{

	int i, k, p, pstatus;
	void *tstatus;

	sem_init(&sem, 0, 0);
	signal(SIGUSR1, h);
	pipe(pd);


	for(i = 0; i<num; i++){

		p = fork();

		if(p==0) {
			//printf("process: %d\n", getpid());

			sem_wait(&sem);
			num++;
			for(k =0; k<num; k++){
		
				arg[k] = k;
				printf("new thread, parent: %d, num: %d, k %d\n", getpid(), num, k);
				pthread_create(&thread[k], NULL, f, (void *) &arg[k]);

			}
			
			for(k =0; k<num; k++) pthread_join(thread[k], &tstatus);
			if(num<4) write(pd[1], &num, sizeof(num));

			exit(res);

		}
		
		printf("process id: %d,p:%d, parent:%d, i: %d num: %d\n", getpid(),p,getppid(),i,num);

			
	
		
	}

	write(pd[1], &num, sizeof(num));
	printf("SIGURS1 %d\n", SIGUSR1);
	signal(SIGUSR1, SIG_IGN);
	kill(0, SIGUSR1);

	for(i = 0; i<num; i++) {
		wait(&pstatus);
		if(WIFEXITED(pstatus)) res += WEXITSTATUS(pstatus);
	}

	printf("final result1: %d \n", res);
	read(pd[0], &num, sizeof(num));
	printf("final result2: %d \n", num);

	return 0;
}


