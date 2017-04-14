//mypthread.c

#include "mypthread.h"

#define pm(msg) printf("%s on %i\n", msg, __LINE__)

/*
* Concept - In order to swap context a thread MUST yield.
* So on a pthread_create we need to run the method - even if it doesn't call a yield and return back to the main thread.
* On an exit we need to stop the thread from running and move back to the our caller thread
* A join is basically a context switch (yield) to the thread which we want to finish with.
* A yield gives up the control to the last scheduled process.
*/

queue_t *qmakequeue()
{
    queue_t *myqueue = (queue_t *)malloc(sizeof(queue_t));
    create_queue(myqueue);
    return myqueue;
}

//Global

// master is the tid of the first process to create a child and does not change.
// A process that exits with tid equal to master will exit the program.
unsigned short master = 0;
// counter will determine the tid of the next thread to be created. This ensures no duplicates.
unsigned short counter = 1;
// tid will store the current thread's id and will be updated upon context switch
unsigned short tid = 1;

// This queue holds threads that have yielded or have entered the ready queue after their join join_tid is satisfied.
// The first process in this queue will be swapped to when the next context switch occurs.
queue_t *ready = NULL;

int compare_tid_pthreads(void *d1, void *d2)
{
    // printf("Comparing %d and %d\n", d1->tid, d2->tid);
    if (((mypthread_t *)d1)->tid == ((mypthread_t *)d2)->tid)
    {
        // printf("Returning 0\n");
        return 0;
    }
    else
    {
        // printf("Returning 1\n");
        return 1;
    }
}

// Type your own functions (void)
// e.g., free up sets of data structures created in your library

/*
* Thread create function
* Create a new thread node and queue it onto the end of the thread queue
*/
int mypthread_create(mypthread_t *thread, const mypthread_attr_t *attr, void *(*start_routine)(void *), void *arg)
{
    if (ready == NULL)
    {
        ready = qmakequeue();
    }
    if (master == 0) // only set master the first time create is called
    {
        tid = counter++;
        master = tid;
    }
    // Get the current context and make a new context using it 
    ucontext_t *current = (ucontext_t *)malloc(sizeof(ucontext_t));
    // printf("Current %p\n", current);
    getcontext(current);
    current->uc_stack.ss_sp = malloc(STACK_SIZE); // Set stack size
    // printf("current ss_sp %p\n", current->uc_stack.ss_sp);
    current->uc_stack.ss_size = STACK_SIZE; // Set pointer size
    current->uc_link = NULL;

    // makecontext to specify the end behavior in order to force 
    // a mypthread_exit via uc_link
    ucontext_t *exit_cont = (ucontext_t *)malloc(sizeof(ucontext_t));
    getcontext(exit_cont);
    exit_cont->uc_stack.ss_sp = malloc(STACK_SIZE);
    exit_cont->uc_stack.ss_size = STACK_SIZE;
    makecontext(exit_cont, (void(*)(void))mypthread_exit, 1, 0);
    current->uc_link = exit_cont;

    // Now make the context with the original start routine.
    makecontext(current, (void(*)())start_routine, 1, arg);
    Node *mnode = malloc(sizeof(Node));
    if (counter + 1 == master)
    {
        counter += 2;
    }

    thread->tid = counter++;
    thread->node = mnode;
    thread->node->mycontext = *current;
    qenqueue(ready, thread); // enqueue the created process into the ready queue, but do not swap to it
    return 0;
}

/*
* Thread exit
* Exit 
* 
*/
void mypthread_exit(void *retval)
{
    // If we are the master (main) thread we exit the process
    // Otherwise simply exit(return) and let uc_link take care of the rest?
    if (tid == master) {
        pm("Exiting main thread.");
        exit(0);
    }

    //Not master thread - go to another thread.
    mypthread_t* newthread;
    qdequeue(ready, (void**)&newthread);
    
    // 
    tid = newthread->tid;
    setcontext(&(newthread->node->mycontext));
    return;
}

/*
* Thread yield function
* 
*/
int mypthread_yield()
{
    // S When we yield we assume that we are the currently running thread.
    // Our TID is NOT in the ready queue.
    // We need to add ourselves to the ready queue
    // Then after adding ourselves to the ready queue we dequeue 
    // the next item and swap contexts

    //Create new node for outself and put in ready queue
    
    mypthread_t* me = malloc(sizeof(mypthread_t));
    me->tid = tid;
    me->node = malloc(sizeof(Node));
    getcontext(&(me->node->mycontext));
    qenqueue(ready, me);
    mypthread_t* newthread;
    
    // Dequeue the next node and prepare to swap contexts
    qdequeue(ready, (void**)&newthread);

    // Set the TID and switch contexts;
    tid = newthread->tid;
    swapcontext(&(me->node->mycontext), &(newthread->node->mycontext));
    return 1;
}

void print_queue(queue_t *q)
{
    // printf("printing queue\n");
    if (q == NULL)
    {
        printf("q is null\n");
        return;
    }
    if (q->rear == NULL)
    {
        printf("Empty queue\n");
        return;
    }
    queue_node *i = q->rear->next;
    do
    {
        i = i->next;
    } while (i != q->rear->next);
    printf("\\ \n");
    return;
}

/*
* Thread join
* Enter wait queue with thread ID of thread to join on.
*/
int mypthread_join(mypthread_t thread, void **retval)
{
    // If the thread that we wish to join on does NOT
    // exist within the ready or waiting queues, then simply continue
    if (qexists(ready, &thread, &compare_tid_pthreads) == 1)
    { // True if exists
        //Now we need to block essentially until thread is not in the ready for waiting queue
        while (qexists(ready, &thread, &compare_tid_pthreads))
        {

            mypthread_yield();
        }
    }
    // Returns here only if qexists == FALSE, otherwise a yield occurs
    return 1;
}
