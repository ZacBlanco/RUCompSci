//mypthread.c

#include "mypthread.h"


/*
* Concept - In order to swap context a thread MUST yield.
* So on a pthread_create we need to run the method - even if it doesn't call a yield and return back to the main thread.
* On an exit we need to stop the thread from running and move back to the our caller thread
* A join is basically a context switch (yield) to the thread which we want to finish with.
* A yield gives up the control to the last scheduled process.
*/


typedef struct threadNode{
    
// Type your commands
// Need field for retval
// Need field for join_thread
// Next threadnode pointer
// mycontext - the context to switch to

}Node;

//Global

// Type your globals...

// Type your own functions (void)
// e.g., free up sets of data structures created in your library

/*
* Thread create function
* Create a new thread node and queue it onto the end of the thread queue
*/
int mypthread_create(mypthread_t *thread, const mypthread_attr_t *attr, void *(*start_routine) (void *), void *arg) {

}

/*
* Thread exit
* Exit 
* 
*/
void mypthread_exit(void *retval) {
  exit(0);
}

/*
* Thread yield function
* 
*/
int mypthread_yield() {

}


/*
* Thread join
* Enter wait queue with thread ID of thread to join on.
*/
int mypthread_join(mypthread_t thread, void **retval) {

}