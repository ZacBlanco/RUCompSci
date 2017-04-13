#include "mypthread.h"

#define NUM_THREADS 10

void *print(void *arg)
{
    printf("THREAD FUNCTION\n");
    mypthread_exit(0);
}

void mypthread_test()
{

    mypthread_t tid;
    printf("Creating thread\n");
    mypthread_create(&tid, NULL, print, NULL);
    printf("Joining on thread\n");
    mypthread_join(tid, NULL);
    
    return;
}

int main()
{
    mypthread_test();
}