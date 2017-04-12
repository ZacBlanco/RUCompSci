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
ucontext_t mycontext // - the context to switch to

}Node;

//Global

// master is the tid of the first process to create a child and does not change.
// A process that exits with tid equal to master will exit the program.
unsigned short master = 0;
// counter will determine the tid of the next thread to be created. This ensures no duplicates.
unsigned short counter = 1;
// tid will store the current thread's id and will be updated upon context switch
unsigned short tid = 1;
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
	if(master == 0) // only set master the first time create is called
	{	
		tid = counter++;
		master = tid;
	}
	unsigned short temp = tid;
	tid = counter++;	// tid is altered so that the new context has the tid of the newly created process.
	ucontext_t current;
	getcontext(&current);
	// makecontext(&current, (*start_routine), 1, arg); // unsure how to use makecontext, need to allocate memory between getcontext and this	
	// enqueue the created process into the ready queue, but do not swap to it (Not written yet)
	tid = temp;		// tid is restored to its original value for the creating process.
	return 0;
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
	while(qexists(wait, &node,(void *) &compare_condition_pthreads))	// search wait queue for processes that have called join
	{								// and are waiting for current process to exit.
		mypthread_t temp_thread;				// if any are found, move them to ready queue
		qdelete_item(wait,&temp_thread,(void *) &compare_condition_pthreads);
		qenqueue(ready,&temp_thread);
	}
	mypthread_t next;
	qdequeue(ready,(void *) &next);
	ucontext_t dummy;
	swapcontext(&dummy, &next.mynode->mycontext);	// swap contexts without storing, effectively terminating current thread
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
	if(qexists(ready,&node,(void *) &compare_tid_pthreads) == 0)	// determine if current process is already in ready queue
	{
		node.mynode->mycontext = current;			// if not, it must be enqueued and the process switches to next in queue
		qenqueue(ready,&node);
		mypthread_t next;
		qdequeue(ready,(void *) &next);
		ucontext_t dummy;
		swapcontext(&dummy, &next.mynode->mycontext);
	}
	else							// current process is in ready queue, indicating that is has returned 
	{							// after yielding. Now it must dequeue itself and continue operation.
		mypthread_t dummy;
		qdequeue(ready,(void *) &dummy);
	}
	return 1;
}


/*
* Thread join
* Enter wait queue with thread ID of thread to join on.
*/
int mypthread_join(mypthread_t thread, void **retval) {
	ucontext_t current;
	getcontext(&current);
	mypthread_t node;
	node.tid = tid;
	if(qexists(ready,&node,(void *) &compare_tid_pthreads) == 0)	// determine if current process is already in ready queue
	{
		if(qexists(ready,&thread,(void *) &compare_tid_pthreads) == 1 || qexists(wait,&thread,(void *) &compare_tid_pthreads) == 1)	
		{							// determine if join process is already in either queue
			node.mynode->mycontext = current;			// if so, enqueue into wait queue as normal	
			node.mynode->condition = thread.tid;
			qenqueue(wait,&node);
			mypthread_t next;
			qdequeue(ready,(void *) &next);
			ucontext_t dummy;
			swapcontext(&dummy, &next.mynode->mycontext);
		}

		return 0;				// if not, return and indicate that thread to join is not in either queue
	}
	else						// current process is in ready queue, indicating that is has returned 
	{						// after joining. Now it must dequeue itself and continue operation.
		mypthread_t dummy;
		qdequeue(ready,(void *) &dummy);
	}
	return 1;

}
