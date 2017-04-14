//mypthread.c
#ifndef H_MYPTHREAD
#define H_MYPTHREAD

#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "queue.h"

#define STACK_SIZE 65536

typedef struct
{
    //Thread ID
    unsigned short tid;
    //Pointer to thread node
    struct threadNode *node;
} mypthread_t;

typedef struct threadNode
{
    //retval somewhere
    ucontext_t mycontext; // - the context of the thread
} Node;

typedef struct
{
    //Not specified to implement in project instructions but decided to do it anyway.
} mypthread_attr_t;

// Functions
int mypthread_create(mypthread_t *thread, const mypthread_attr_t *attr, void *(*start_routine)(void *), void *arg);

void mypthread_exit(void *retval);

int mypthread_yield();

int mypthread_join(mypthread_t thread, void **retval);
void print_queue(queue_t *q);

/* Don't touch anything after this line.
 *
 * This is included just to make the mtsort.c program compatible
 * with both your ULT implementation as well as the system pthreads
 * implementation. The key idea is that mutexes are essentially
 * useless in a cooperative implementation, but are necessary in
 * a preemptive implementation.
 */

typedef int mypthread_mutex_t;
typedef int mypthread_mutexattr_t;

static inline int mypthread_mutex_init(mypthread_mutex_t *mutex,
                                       const mypthread_mutexattr_t *attr) { return 0; }

static inline int mypthread_mutex_destroy(mypthread_mutex_t *mutex) { return 0; }

static inline int mypthread_mutex_lock(mypthread_mutex_t *mutex) { return 0; }

static inline int mypthread_mutex_trylock(mypthread_mutex_t *mutex) { return 0; }

static inline int mypthread_mutex_unlock(mypthread_mutex_t *mutex) { return 0; }

#endif
