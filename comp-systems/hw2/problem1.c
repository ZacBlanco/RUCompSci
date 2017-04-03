#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
// Sieve of Eratosthenes

pthread_mutex_t numlock = PTHREAD_MUTEX_INITIALIZER;

struct sieve_node {
	struct loopargs* args;
	pthread_t tid;
	struct sieve_node* next;
};
struct loopargs {
	int start_index;
	int n;
	int* nums;
};


void* sieve_worker(struct loopargs* args);
void* rev_printer(struct loopargs* args);



/* ============= DOCUMENTATION ===================


	Description:
		Give an input integer, n, this program will print to the console a list of all
		prime numbers up to n. After which is will print the reversed primes


	Expected Arguments:
		(int) - A number which limits the primes we output. A number MUST be supplied
		and must be > 0


	Examples:
		gcc -o sieve  problem1.c -lm -pthread

		./sieve 10
		Primes are:
		2 is prime
		3 is prime
		5 is prime
		7 is prime
		Reversed Prime: 2
		Reversed Prime: 3
		Reversed Prime: 5
		Reversed Prime: 7

*/
int main(int argc, char* argv[]) {
	if (argc < 2){
		printf("At least 1 argument required\n");
		exit(1);
	}
	// The max prime
	int n = atoi(argv[1]);
	
	if (n < 1) {
		printf("argument must be > 0\n");
		exit(1);
	}
	
	// Set all values to TRUE
	int* nums = (int*) malloc(sizeof(int)*n);
	int i, j;
	float lim = sqrt(n);
	for(i = 0; i < n; i++) {
		nums[i] = 1;
	}

	// The Sieve of Eatosthenes
	struct sieve_node* head;
	struct sieve_node*  last;
	head = NULL;
	last = NULL;
	for(i = 2; i < lim; i++) {
		if (nums[i] == 1) {
			//Loop over i^2, i^2 + 1, i^2 + 2i, etc..
			// Use the thread worker function

			// LL Pointer
			struct sieve_node* tmp = malloc(sizeof(struct sieve_node));
			struct loopargs* la = malloc(sizeof(struct loopargs));
			
			la->start_index = i;
			la->n = n;
			la->nums = nums;
			tmp->args = la;
			tmp->next = NULL;
			//printf("Creating thread\n");
			pthread_create(&(tmp->tid), NULL, &sieve_worker, la);
			//Set list pointers approrpiately.
			if (last == NULL) {
				last = tmp;
			} else {
				last->next = tmp;
				last = tmp;
			}
			if (head == NULL){
				head = last;
			}
			
		}
	}

	//Free all dynamically allocated memory
	struct sieve_node* curr = head;
	struct sieve_node* tmp = NULL;
	while(curr != NULL) {
		pthread_join(curr->tid, NULL);
		tmp = curr->next;
		free(curr->args);
		free(curr);
		curr = tmp; //Essentially advancing LL pointer
	}
	printf("Primes are:\n");
	//Loop and print the primes
	for(i = 2; i < n; i++){
		if(nums[i] == 1){
			printf("%i is prime\n", i);
		}
	}

	//Print reversed numbers
	pthread_t rev;
	struct loopargs revargs = {0, n, nums};
	pthread_create(&rev, NULL, &rev_printer, &revargs);
	pthread_join(rev, NULL);		

	free(nums);
}

void* rev_printer(struct loopargs* args){
	int i;
	for(i = 2; i < args->n; i++){
		if ( (args->nums)[i] == 0){
			continue;
		}
		// Reverses the number and prints
		int rn = 0;
		int tmp = i;
		while (tmp != 0) {
			rn = rn*10 + (tmp % 10);
			tmp /= 10;
		} 
		printf("Reversed Prime: %i\n", rn);
	}	
}

void* sieve_worker(struct loopargs* args) {
	int i = args->start_index;
	//printf("Marking false");
	for (i *= args->start_index; i < args->n; i += args->start_index){
		pthread_mutex_lock(&numlock);
		(args->nums)[i] = 0;
		pthread_mutex_unlock(&numlock);
	}
}


