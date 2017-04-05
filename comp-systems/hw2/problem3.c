#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

sem_t sem;

void make_transaction()
{
	printf("Customer is making a transaction\n");
}

void take_a_walk()
{
	printf("Customer is waiting\n");
}

void return_home()
{
	printf("Customer has left\n");
}

void* bank_client(void* arg)
{
	while (1) 
	{
		if (sem_wait(&sem) == 0)
		{
			printf("%s\n", strerror(errno));
			make_transaction();
			sem_post(&sem);
			break;
		} 
		else
		{
			sleep(1);
			take_a_walk();
		}
	}
	return_home();
}

int main(int argc, char *argv[])
{
	if (argc != 3) 
	{
		printf("Incorrect # args\n");
		return 1;
	}
	int empty_seats = atoi(argv[1]);
	int total_customers = atoi(argv[2]);

	sem_init(&sem, 0, empty_seats);
	pthread_t threads[total_customers];
	int i;
	for (i = 0; i < total_customers; ++i)
	{
		pthread_create(&threads[i], NULL, bank_client, NULL);
	}

	for (i = 0; i < total_customers; ++i)
	{
		pthread_join(threads[i], NULL);
	}

	sem_destroy(&sem);
}