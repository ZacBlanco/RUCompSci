//mypthread.c

#include "mypthread.h"
#include "queue.h"

#define STACK_MEM 65536

/*
* Concept - In order to swap context a thread MUST yield.
* So on a pthread_create we need to run the method - even if it doesn't call a yield and return back to the main thread.
* On an exit we need to stop the thread from running and move back to the our caller thread
* A join is basically a context switch (yield) to the thread which we want to finish with.
* A yield gives up the control to the last scheduled process.
*/

queue_t* qmakequeue()
{
	queue_t* myqueue = (queue_t*)malloc(sizeof(queue_t));
	create_queue(&myqueue);
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
// This queue will hold all threads that are currently waiting for another to exit via the join function.
queue_t* wait = NULL;

// This queue holds threads that have yielded or have entered the ready queue after their join condition is satisfied.
// The first process in this queue will be swapped to when the next context switch occurs.
queue_t* ready = NULL;


int compare_tid_pthreads(mypthread_t* d1, mypthread_t* d2) {
	printf("Comparing %d and %d\n", d1->tid, d2->tid);
	if (d1->tid == d2->tid) {
		printf("Returning 0\n");
		return 0;
	}
	else {
		printf("Returning 1\n");
		return 1;
	}
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
	if (wait == NULL) {
		wait = qmakequeue();
	}
	if (ready == NULL) {
		ready = qmakequeue();	
	}
	if(master == 0) // only set master the first time create is called
	{	
		tid = counter++;
		master = tid;
	}
	unsigned short temp = tid;
	tid = counter++;	// tid is altered so that the new context has the tid of the newly created process.
	ucontext_t* current = (ucontext_t*)malloc(sizeof(ucontext_t));
	getcontext(current);
	current->uc_stack.ss_sp = malloc(sizeof(STACK_SIZE));
	current->uc_stack.ss_size = STACK_SIZE;
	current->uc_link = current; // Optimistic solution
	makecontext(current, (*start_routine), 1, arg);
	//mypthread_t* newnode = (mypthread_t*)malloc(sizeof(mypthread_t));
	//newnode->tid = tid;
	Node* mnode = malloc(sizeof(Node));
	thread->tid = tid;
	thread->mynode = mnode;
	thread->mynode->mycontext = current;
	//newnode->mynode = mnode;
	//newnode->mynode->mycontext = current;
	qenqueue(ready,thread);// enqueue the created process into the ready queue, but do not swap to it 
	tid = temp;		// tid is restored to its original value for the creating process.
	return 0;
}

/*
* Thread exit
* Exit 
* 
*/
void mypthread_exit(void *retval) {
	printf("In exit\n");
	if(tid == master) {
		printf("Exiting tid: %d, master: %d\n", tid, master);
		exit(0);
	}
	mypthread_t node;
	//node->mynode->condition = tid;
	node.tid = tid;
	while(qexists(wait, &node,(void *) &compare_condition_pthreads))	// search wait queue for processes that have called join
	{								// and are waiting for current process to exit.
		mypthread_t temp_thread;				// if any are found, move them to ready queue
		qdelete_item(wait,&temp_thread,(void *) &compare_condition_pthreads);
		qenqueue(ready,&temp_thread);
	}
	printf("Finished moving\n");
	mypthread_t* next;
	next = ready->rear->next->data;
	ucontext_t dummy;
	printf("Exit: swapping context\n");
	swapcontext(&dummy, next->mynode->mycontext);	// swap contexts without storing, effectively terminating current thread
	// should never reach here	
	exit(0);
}

/*
* Thread yield function
* 
*/
int mypthread_yield() {
	ucontext_t* current;
	getcontext(current);
	mypthread_t* node;
	node->tid = tid;
	if(qexists(ready,&node,(void *) &compare_tid_pthreads) == 0)	// determine if current process is already in ready queue
	{
		mypthread_t* newnode = (mypthread_t*)malloc(sizeof(mypthread_t));
		newnode->tid = tid;
		Node* mnode = malloc(sizeof(Node));
		newnode->mynode = mnode;
		newnode->tid = tid;
		newnode->mynode->mycontext = current;	// if not, it must be enqueued and the process switches to next in queue
		qenqueue(ready,&node);
		
		mypthread_t* next;
		next = ready->rear->next->data;
		ucontext_t dummy;
		swapcontext(&dummy, next->mynode->mycontext);
	}
	else							// current process is in ready queue, indicating that is has returned 
	{							// after yielding. Now it must dequeue itself and continue operation.
		mypthread_t* dummy;
		qdequeue(ready,(void *) &dummy);
		
	}
	return 1;
}


/*
* Thread join
* Enter wait queue with thread ID of thread to join on.
*/
int mypthread_join(mypthread_t thread, void **retval) {
	printf("Printing waiting queue\n");
	print_queue(wait);
	printf("Printing ready queue\n");
	print_queue(ready);

	ucontext_t* current;
	//printf("Current ptr: %p\n", current);
	//printf("ABOUT TO JOIN A THREAD WITH TID: %i\n", tid);	
	getcontext(&current);
	//printf("Current ptr after: %p\n", current);
	//printf("JOINING A THREAD WITH TID: %i\n", tid);	
	mypthread_t node;	
	node.tid = tid;
	printf("The current tid is %d\n", node.tid);
	if(qexists(ready, &node,(void *) &compare_tid_pthreads) == 0)	// determine if current process is already in ready queue
	{
		printf("The current process is not in rear queue\n");
		if(qexists(ready,&thread,(void *) &compare_tid_pthreads) == 1 || qexists(wait,&thread,(void *) &compare_tid_pthreads) == 1)	
		{							// determine if join process is already in either queue
			printf("The current process is not in either queue\n");
			mypthread_t* newnode = (mypthread_t*)malloc(sizeof(mypthread_t));
			Node* mnode = malloc(sizeof(Node));
			newnode->mynode = mnode;
			newnode->tid = tid;
			newnode->mynode->mycontext = current;			// if so, enqueue into wait queue as normal	
			newnode->mynode->condition = thread.tid;
			qenqueue(wait,&node);

			mypthread_t* next;
			next = ready->rear->next->data;
			ucontext_t dummy;
			printf("The next tid is: %d\n", next->tid);
			tid = next->tid;
			swapcontext(&dummy, next->mynode->mycontext);
		}

		return 0;				// if not, return and indicate that thread to join is not in either queue
	}
	else						// current process is in ready queue, indicating that is has returned 
	{						// after joining. Now it must dequeue itself and continue operation.
		printf("The current process is in ready queue\n");
		mypthread_t dummy;
		qdequeue(ready,(void *) &dummy);
	}
	return 1;

}
