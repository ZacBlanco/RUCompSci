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
short condition;
// Next threadnode pointer
// mycontext - the context to switch to

}Node;

//Global

// master is the tid of the first process to create a child and does not change.
// A process that exits with tid equal to master will exit the program.
unsigned short master = -1;
// counter will determine the tid of the next thread to be created. This ensures no duplicates.
unsigned short counter = 0;
// This queue will hold all threads that are currently waiting for another to exit via the join function.
queue_t* wait;
// This queue holds threads that have yielded or have entered the ready queue after their join condition is satisfied.
// The first process in this queue will be swapped to when the next context switch occurs.
queue_t* ready;


int compare_tid_pthreads(mypthread_t* d1, mypthread_t* d2) {
	if (d1->tid == d2->tid)
		return 1;
	else
		return 0;
}

int compare_condition_pthreads(mypthread_t* d1, mypthread_t* d2) {
	if (d1->mynode->condition == d2->mynode->condition)
		return 1;
	else
		return 0;
}




// Type your own functions (void)
// e.g., free up sets of data structures created in your library

/*
* Thread create function
* Create a new thread node and queue it onto the end of the thread queue
*/
int mypthread_create(mypthread_t *thread, const mypthread_attr_t *attr, void *(*start_routine) (void *), void *arg) {
	if(master == -1) // only set master the first time create is called
	{	
		tid = counter++;
		master = tid;
	}
	temp = tid;
	tid = counter++;	// tid is altered so that the new context has the tid of the newly created process.
	ucontext_t current;
	getcontext(&current);
	makecontext(&current, (*start_routine), 1, arg);	
	// enqueue the created process into the ready queue, but do not swap to it (Not written yet)
	tid = temp;		// tid is restored to its original value for the creating process.
}

/*
* Thread exit
* Exit 
* 
*/
void mypthread_exit(void *retval) {
	if(tid == master)
		exit(0);
	mypthread_t node;
	node.mynode->condition = tid;
	while(qexists(&wait, &node, &compare_condition_pthreads)	// search wait queue for processes that have called join
	{								// and are waiting for current process to exit.
		mypthread_t temp_thread;				// if any are found, move them to ready queue
		qdelete_item(&wait,&temp_thread,&compare_condition_pthreads);
		qenqueue(&ready,&temp_thread);
	}
	mypthread_t next;
	dequeue(&ready,&next);
	swapcontext(next.mynode->mycontext);	// swap contexts without storing, effectively terminating current thread
	// should never reach here	
	exit(0);
}

/*
* Thread yield function
* 
*/
int mypthread_yield() {
	ucontext_t current;
	getcontext(&current);
	mypthread_t node;
	node.tid = tid;
	if(qexists(&ready,&node,&compare_tid_pthreads) == 0)	// determine if current process is already in ready queue
	{
		node.mynode->context = current;			// if not, it must be enqueued and the process switches to next in queue
		qenqueue(&ready,&node);
		mypthread_t next;
		dequeue(&ready,&next);
		swapcontext(next.mynode->mycontext);
	}
	else							// current process is in ready queue, indicating that is has returned 
	{							// after yielding. Now it must dequeue itself and continue operation.
		mypthread_t dummy;
		dequeue(&ready,&dummy);
	}
	return 1;
}


/*
* Thread join
* Enter wait queue with thread ID of thread to join on.
*/
int mypthread_join(mypthread_t thread, void **retval) {

}