#include "mypthread.h"

#define NUM_THREADS 10

void *print(void *arg)
{
    printf("THREAD FUNCTION\n");
    mypthread_exit(0);
}

void mypthread_test()
{

    mypthread_t tid[NUM_THREADS];
    int i;
    printf("Creating all threads\n");
    for (i = 0; i < NUM_THREADS; i++)
    {
        mypthread_create(&tid[i], NULL, print, NULL);
    }
    printf("Joining threads\n");
    for (i = 0; i < NUM_THREADS; i++)
    {
        printf("Joining on thread %d\n", i);
        mypthread_join(tid[i], NULL);
    }
    printf("Finished joining thread\n");
    return;
}

int main()
{
    mypthread_test();
}