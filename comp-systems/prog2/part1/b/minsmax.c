#include "minsmax.h"

int main_stats[3] = {INT_MIN, INT_MAX, 0};
int num_sigs = 0;
struct p_times* child_procs;
int total_ptimes;

void sigq_err(int ret) {
    printf("Sigqueue in process %i: %i\n", getpid(), ret);
    if (ret == -1) {
      perror("Sigqueue Err:");
    }
}

void send_data(int* data, int pauses) {
    // Instead of writing we send the signal:
    printf("PID %i waiting for %i signals, num_sigs %i\n", getpid(), pauses, num_sigs);
    
    int i;
    for (i = 0; i< pauses; i++) {
      pause();
      printf("pause%i done\n", i);
    }
    union sigval val;
    val.sival_int = data[0];
    sigq_err(sigqueue(getppid(), 10, val));
    sleep(0.2);
    val.sival_int = data[1];
    sigq_err(sigqueue(getppid(), 16, val));
    sleep(0.2);
    val.sival_int = data[2];
    sigq_err(sigqueue(getppid(), 30, val));
    sleep(2.0);
    
}

void minsmax_signal_handler(int sig, siginfo_t* data, void* context) {
  // SIGUSR1 ==> 30, 10, or 16
  // process min if 10
  // process max if 16
  // process sum if 30
  if (sig == 10) {

    if (data->si_int < main_stats[0]) {
      main_stats[0] = data->si_int;
    }

  } else if (sig == 16) {
    
    if (data->si_int > main_stats[1]) {
      main_stats[1] = data->si_int;
    }

  } else if (sig == 30) {
    main_stats[2] += data->si_int;
    
    // Get signal handler and set completed for sending pid = true
    // Only set completed after all 3 are sent (30 is last)
    int i;
    for(i = 0; i < total_ptimes; i++) {
      if (data->si_pid == child_procs[i].pid) {
        child_procs[i].finished = 1;
      }
    }

  
  }
  num_sigs++;

  printf("PID: %i Got signal: %i. num_sigs: %i\n", getpid(), sig, num_sigs);
}

void minsmax_impatient_handler(int sig, siginfo_t* data, void* context) {
    kill(data->si_int, SIGKILL);
}

void minsmax(const int* data, int len, int* results) {

  if(len <= 0) { return; }

  int sum = 0;
  int max = INT_MIN;
  int min = INT_MAX;
  int i;
  for(i = 0; i < len; i++) {
    if(data[i] > max) {
      max = data[i];
    }

    if(data[i] < min) {
      min = data[i];
    }

    sum += data[i];
  }
  //Write into array as
  // +-----+-----+-----+
  // | MIN | MAX | SUM |
  // +-----+-----+-----+
  // printf("minsmax: min, max, sum: %i, %i, %i\n", min, max, sum);
  results[0] = min;
  results[1] = max;
  results[2] = sum;
}

void print_stats(struct stats stat) {
  printf("The min is %i\n", stat.min);
  printf("The max is %i\n", stat.max);
  printf("The sum is %i\n", stat.sum);
  fflush(stdout);
}

void recurse_minsmax_helper(int* data, int num_proc, int data_length) {
  struct sigaction minsmax_sigact;
  minsmax_sigact.sa_flags = SA_SIGINFO;
  minsmax_sigact.sa_sigaction = &minsmax_signal_handler;
  sigaction(10, &minsmax_sigact, NULL);
  sigaction(16, &minsmax_sigact, NULL);
  sigaction(30, &minsmax_sigact, NULL);

  if(num_proc == 0) { 
    exit(0);
  }

  int* stats = &main_stats;
  if (PROC_OUT) {
    printf("I am a child process %i my parent is %i\n", getpid(), getppid());
    fflush(stdout);
  }

  int fpid = fork();

  if (fpid < 0) {
    //err
    perror("Error making child process");
    exit(1);
  } else if (fpid == 0) {
    // child
    main_stats[0] = INT_MIN;
    main_stats[1] = INT_MAX;
    main_stats[2] = 0;
    int* start = data + data_length;
    recurse_minsmax_helper(start, num_proc - 1, data_length);

  } else {
    // parent
    // Do calculations
    minsmax(data, data_length, stats);
    // Get other data
    waitpid(fpid, NULL, 0);
    printf("pid: %i numproc %i", getpid(), num_proc);
    send_data(main_stats, num_proc);
    exit(0);
  }
  
}

struct stats main_iter_recur_minsmax(char* file, int proc_breadth, int proc_depth) {
  // SET UP THE SIGNAL HANDLER
  struct sigaction minsmax_sigact;
  minsmax_sigact.sa_flags = SA_SIGINFO;
  minsmax_sigact.sa_sigaction = &minsmax_signal_handler;
  sigaction(10, &minsmax_sigact, NULL); // SIGUSR1
  sigaction(16, &minsmax_sigact, NULL); // SIGUSR1
  sigaction(30, &minsmax_sigact, NULL); // SIGUSR1

  // handler for impatient processes
  minsmax_sigact.sa_sigaction = &minsmax_impatient_handler;
  sigaction(31, &minsmax_sigact, NULL); // SIGUSR2

  // Initialize
  int* ints = NULL; // ints is the list of numbers
  int* stats = &main_stats;     // stats array represents the min,max,sum statistic

  // Check num_proc for errors
  if (proc_breadth < 1 || proc_depth < 1) {
    perror("Number of processes must be >= 1");
    exit(1);
  }

  // Read numbers in and check for errors
  int n; // n is the number of integers read in
  if ((n = readFile(file, &ints)) == -1) {
    perror("Bad File Input");
    exit(1);
  }

  if (PROC_OUT) {
    printf("I am the main process and my pid is %i\n", getpid());
    fflush(stdout);
  }

  if (proc_breadth > n) {
    proc_breadth = n;
  }

  int len = n / proc_breadth;
  int main_len = (n % proc_breadth) + len;

  if (proc_depth > len) {
    proc_depth = len;
  }

  int recur_len = len / proc_depth;
  int main_recur_len = (len % proc_depth) + recur_len;

  // printf("main_len: %i, len: %i, recur_len: %i, main_recur_len: %i\n", main_len, len, recur_len, main_recur_len);
  // Fork into children
  // Do a preliminary write as the base case or whatever
  int* start = ints + main_len;
  int i;

  // Array of structs for PIDS
  // Total # is proc_breadth-1;
  struct p_times times[proc_breadth-1];
  child_procs = times;
  total_ptimes = proc_breadth-1;


  for (i = 1; i < proc_breadth; i++)
  {
    time_t ft;
    time(&ft); //fork time
    int fpid = fork();
    if (fpid < 0) {
      perror("Error making child process");
      exit(1);
    } else if (fpid == 0) {
      main_stats[0] = INT_MIN;
      main_stats[1] = INT_MAX;
      main_stats[2] = 0;
      if (PROC_OUT) {
        printf("I am a child process %i my parent is %i\n", getpid(), getppid());
        fflush(stdout);
      }

      // Get data for this process
      // Proc depth - 1 because we must account for the main process
      int fpid2 = fork();
      if (fpid2 < 0 ) { 
        perror("Error doing fork in iterative recursive mode\n");
        exit(1);
      } else if (fpid2 == 0) {
        main_stats[0] = INT_MIN;
        main_stats[1] = INT_MAX;
        main_stats[2] = 0;
        recurse_minsmax_helper(start+main_recur_len, proc_depth - 1, recur_len);
      }
      minsmax(start, main_recur_len, stats);
      // Get other data
      waitpid(fpid2, NULL, 0);

      printf("PID: %i, stats after recur %i: min: %i, max: %i, sum %i\n", getpid(), i, stats[0], stats[1], stats[2]);
      // Instead of writing we send the signal:
      send_data(main_stats, proc_depth-1);
      exit(0);
    } else {
      // We are parent so we need to store the struct corresponding to our child.
      times[i-1].pid = fpid;
      times[i-1].stime = ft;
      times[i-1].finished = 0;
      start += len;
      sleep(0.15);
    }
  }
  minsmax(ints, main_len, stats);
  
  struct stats ret2 = { stats[0], stats[1], stats[2] };
  for(i = 0; i < proc_breadth-1;i++) {
    int j;
    time_t curtime;
    time(&curtime);

    // For each process check if it has been > 3 seconds
    for (j = 0; j < total_ptimes; j++) {

      // If not finished
      if (child_procs[i].finished == false && curtime - child_procs[i].stime > 3) {
        int k;
        for (k=0; k < proc_breadth-1; k++) {
          if (k != j) {
            union sigval val;
            val.sival_int = child_procs[i].pid;
            sigqueue(child_procs[k].pid, 31, val);
          }
          
        }
        kill(child_procs[j].pid, SIGKILL);
      }
    }    
    wait(NULL);
  }

  //busy wait
  while(num_sigs < 3*(proc_breadth-1));
  
  struct stats ret = { stats[0], stats[1], stats[2] };
  return ret;
}




