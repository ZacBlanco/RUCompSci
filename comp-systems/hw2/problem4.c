#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

pthread_mutex_t allLock = PTHREAD_MUTEX_INITIALIZER;

#define R 8

int teacherCount = 0;
int childCount = 0;

void teacher_enter(sem_t *semPtr)
{
    
    for(int i = 0; i < R; i++)
    {
        sem_post(semPtr);
    }
    pthread_mutex_lock(&allLock);
    teacherCount++;
    pthread_mutex_unlock(&allLock);
    return;
}

void teacher_exit(sem_t *semPtr)
{
    if(teacherCount <= 0)
    {
        printf("# of teachers can't be less than 0\n");
        return;
    }
    
    pthread_mutex_lock(&allLock);
    int sval;
    sem_getvalue(semPtr, &sval);
    if(sval >= R)
    {
        for(int i = 0; i < R; i++)
        {
            sem_wait(semPtr);
        }
        teacherCount--;
    }
    else
    {
        printf("Returning to office.");
        // Return in Office
    }
    pthread_mutex_unlock(&allLock);
    return;
}

void Teacher(sem_t *semPtr)
{
    sleep(1);
    teacher_enter(semPtr);
    printf("teach 1\n");
    teacher_enter(semPtr);
    printf("teach 2\n");
    teacher_exit(semPtr);
    printf("teach 3\n");
    teacher_exit(semPtr);
    printf("teach 4\n");
    teacher_exit(semPtr);
    printf("TeacherDone\n");
    return;
}

void child_enter(sem_t *semPtr)
{
    
    sem_wait(semPtr);
    pthread_mutex_lock(&allLock);
    childCount++;
    pthread_mutex_unlock(&allLock);
    return;
}

void child_exit(sem_t *semPtr)
{
     if(childCount <= 0)
    {
        printf("# of children can't be less than 0\n'");
        return;
    }

    sem_post(semPtr);
    pthread_mutex_lock(&allLock);
    childCount--;
    pthread_mutex_unlock(&allLock);
    return;
}

void Child(sem_t *semPtr)
{
    
    child_enter(semPtr);

    child_enter(semPtr);
    sleep(1);
    child_exit(semPtr);
    child_exit(semPtr);
    child_exit(semPtr);
    printf("ChildDone\n");
    return;
}

void verify_compliance(sem_t *semPtr)
{
    pthread_mutex_lock(&allLock);
    int sval;
    sem_getvalue(semPtr, &sval);
    int sval2 = R * teacherCount - childCount;
    printf("sval = %i\nR*T-C = %i\n", sval, sval2);
    pthread_mutex_unlock(&allLock);
    return;
}



int main()
{
    sem_t tcExcess;
    sem_init(&tcExcess,0,0);
    pthread_t tids[3];
    // Teacher thread
    pthread_create(&tids[1], NULL, &Teacher, &tcExcess);
    // Child thread
    pthread_create(&tids[2], NULL, &Child, &tcExcess);
    verify_compliance(&tcExcess);
    sleep(1);
    verify_compliance(&tcExcess);
    pthread_join(&tids[1],NULL);
    pthread_join(&tids[2],NULL);
    sleep(1);
    verify_compliance(&tcExcess);

    printf("TCount = %i\nCCount = %i\n",teacherCount,childCount);

    return 0;
}