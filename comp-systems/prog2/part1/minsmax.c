#include "minsmax.h"

int main_stats[3] = {INT_MIN, INT_MAX, 0};

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
  }

  printf("GOT SIGQUEUE SIGNAL \n");
}

struct stats main_minsmax(char* file) {

  if (PROC_OUT){
    printf("I am the main process and my pid is %i\n", getpid());
    fflush(stdout);
  }
  int* ints = NULL;
  int n = readFile(file, &ints);

  if(n == -1) {
    perror("Bad File Input.");
    exit(1);
  }
  int nums[3];
  minsmax(ints, n, nums);
  struct stats stat = { nums[0], nums[1], nums[2] };
  return stat;
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

struct stats main_recurse_minsmax(char* file, int num_proc) {
  int* ints = NULL;
  int* stats = &main_stats;
  if (PROC_OUT) {
    printf("I am the main process and my pid is %i\n", getpid());
    fflush(stdout);
  }

  if( num_proc <= 0 ) {
    perror("Number of processes must be > 1");
    exit(1);
  }

  int n; // Number of ints read.
  if( (n = readFile(file, &ints)) == -1) {
    perror("Bad File Input.");
    exit(1);
  }

  if( num_proc > n) {
    num_proc = n;
  }

  int len = n / num_proc; // # of ints for all other processes
  int main_len = (n % num_proc) + len; // length of processing for main process

  int fpid = fork();

  if (fpid < 0) {
    //err
    perror("Error making child process");
    exit(1);
  } else if (fpid == 0) {
    // child
    int* start = ints + main_len;
    recurse_minsmax_helper(start, num_proc - 1, len);
    exit(0);

  } else {
    // parent
    // Do calculations
    minsmax(ints, main_len, stats);
    // Get other data
    waitpid(fpid, NULL, 0);
    int odata[3];
    if(odata[0] < stats[0]) { stats[0] = odata[0]; } // Set new min if needed
    if(odata[1] > stats[1]) { stats[1] = odata[1]; } // Set new max if needed
    stats[2] += odata[2];

  }

  struct stats fin = { stats[0], stats[1], stats[2] };

  return fin;
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
    int* start = data + data_length;
    recurse_minsmax_helper(start, num_proc - 1, data_length);

  } else {
    // parent
    // Do calculations
    minsmax(data, data_length, stats);
    // Get other data
    waitpid(fpid, NULL, 0);

    // By this point we should have had signals sent, updating
    
    // Instead of writing we send the signal:
    union sigval val;
    val.sival_int = main_stats[0];
    sigqueue(getppid(), 10, val);
    sleep(0.05);
    val.sival_int = main_stats[1];
    sigqueue(getppid(), 16, val);
    sleep(0.05);
    val.sival_int = main_stats[2];
    sigqueue(getppid(), 30, val);
    sleep(0.05);
    // bytes = write(wr_pipe, (void*)stats, sizeof(int)*3);
    exit(0);
  }
  
}

struct stats main_iter_minsmax(char* file, int num_proc) {
  // Initialize
  int* ints = NULL; // ints is the list of numbers
  int stats[3];     // stats represents the min,max,sum statistic
  int pipes[2];     // pipes represents the read(0)/write(1) pipes

  // Setup pipe and check for errors
  if (pipe(pipes) < 0) {
    perror("Error creating pipes");
    exit(1);
  }

  // Check num_proc for errors
  if (num_proc < 1) {
    perror("Number of processes must be >= 1");
    exit(1);
  }

  // Read numbers in and check for errors
  int n; // n is the number of integers read in
  if ((n = readFile(file, &ints)) == -1) {
    perror("Bad File Input");
    exit(1);
  }

  // Make sure that num_proc isn't higher than n
  if (num_proc > n) {
    num_proc = n;
  }

  if (PROC_OUT) {
    printf("I am the main process and my pid is %i\n", getpid());
    fflush(stdout);
  }
  int len = n / num_proc;
  int main_len = (n % num_proc) + len;

  // Fork into children
  // Do a preliminary write as the base case or whatever
  write(pipes[1], "\0", 1);
  int* start = ints + main_len;
  int i;
  for (i = 1; i < num_proc; i++)
  {
    int fpid = fork();
    if (fpid < 0) {
      perror("Error making child process");
      exit(1);
    } else if (fpid == 0) {
      if (PROC_OUT) {
        printf("I am a child process %i my parent is %i\n", getpid(), getppid());
        fflush(stdout);
      }

      // Get data for this process
      minsmax(start, len, stats);
      // Get other data
      int odata[3];
      ssize_t bytes = read(pipes[0], (void*)odata, sizeof(int)*3);
      // Check if data exists
      if( bytes == 1 ) {
        odata[0] = INT_MAX;
        odata[1] = INT_MIN;
        odata[2] = 0;
      } else if (bytes != sizeof(int)*3) {
        perror("Did not read the correct number of bytes from the pipe");
        exit(1);
      }
      // Merge data into stats, and then write it to pipe
      if(odata[0] < stats[0]) { stats[0] = odata[0]; } // Set new min if needed
      if(odata[1] > stats[1]) { stats[1] = odata[1]; } // Set new max if needed
      stats[2] += odata[2];
      bytes = write(pipes[1], (void*)stats, sizeof(int)*3);
      // Print stats
      close(pipes[0]);
      close(pipes[1]);
      exit(0);
    } else {
      start += len;
    }
  }

  for(i = 0; i < num_proc -1;i++) {
    wait(NULL);
  }
  minsmax(ints, main_len, stats);
  int odata[3];
  ssize_t bytes = read(pipes[0], &odata, sizeof(int)*3);
  // In the case that the parent finishes first,
  if( bytes == 1 ) {
    odata[0] = INT_MAX;
    odata[1] = INT_MIN;
    odata[2] = 0;
  } else if (bytes != sizeof(int)*3) {
    perror("Did not read the correct number of bytes from the pipe");
    exit(1);
  }
  if(odata[0] < stats[0]) { stats[0] = odata[0]; } // Set new min if needed
  if(odata[1] > stats[1]) { stats[1] = odata[1]; } // Set new max if needed
  stats[2] += odata[2];

  
  close(pipes[0]);
  close(pipes[1]);
  struct stats ret = {stats[0], stats[1], stats[2]};
  return ret;
}



struct stats main_iter_recur_minsmax(char* file, int proc_breadth, int proc_depth) {
  // SET UP THE SIGNAL HANDLER
  struct sigaction minsmax_sigact;
  minsmax_sigact.sa_flags = SA_SIGINFO;
  minsmax_sigact.sa_sigaction = &minsmax_signal_handler;
  sigaction(10, &minsmax_sigact, NULL); // SIGUSR
  sigaction(16, &minsmax_sigact, NULL);
  sigaction(30, &minsmax_sigact, NULL);

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
  for (i = 1; i < proc_breadth; i++)
  {
    int fpid = fork();
    if (fpid < 0) {
      perror("Error making child process");
      exit(1);
    } else if (fpid == 0) {
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
        recurse_minsmax_helper(start+main_recur_len, proc_depth - 1, recur_len);
      }
      minsmax(start, main_recur_len, stats);
      // Get other data
      waitpid(fpid2, NULL, 0);

      printf("Stats after recur %i: min: %i, max: %i, sum %i\n", i, stats[0], stats[1], stats[2]);
      // bytes = write(pipes[1], (void*)stats, sizeof(int)*3);
      // Instead of writing we send the signal:
      printf("My ppid: %i\n", getppid());
      union sigval val;
      val.sival_int = stats[0];
      sigqueue(getppid(), 10, val);
      sleep(0.05);
      val.sival_int = stats[1];
      sigqueue(getppid(), 16, val);
      sleep(0.05);
      val.sival_int = stats[2];
      sigqueue(getppid(), 30, val);
      sleep(0.05);
      printf("Sigqueue return: %i\n", sigqueue(getppid(), SIGUSR1, val));
      perror("Sigqueue Err:");
      printf("recurse finished gracefully\n");
      exit(0);
    } else {
      start += len;
    }
  }
  minsmax(ints, main_len, stats);
  printf("My PID %i\n", getpid());
  struct stats ret2 = { stats[0], stats[1], stats[2] };
  print_stats(ret2);
  for(i = 0; i < proc_breadth-1;i++) {
    wait(NULL);
  }
  printf("My PID %i\n", getpid());
  struct stats ret = { stats[0], stats[1], stats[2] };
  return ret;
}




