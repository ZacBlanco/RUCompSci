#include "minsmax.h"

int* gen_rand_array(const int min, const int max, const int n) {
  int* ints = NULL;
  if (n < 1) {
    // Bad size
    perror("Invalid array size");
    return NULL;
  } else if (n == 1) {
    // If there is only one element, min must equal max
    if (min != max) {
      perror("Bad parameters");
      return NULL;
    }
    ints = malloc(sizeof(int));
    ints[0] = min;
  } else {
    // Generate random numbers for the entire array within the range [min,max]
    ints = malloc(sizeof(int)*n);
    time_t t; 
    srand((unsigned) time(&t));
    int mod = max - min + 1;
    int i;
    for (i = 0; i < n; i++) {
      ints[i] = (rand() % mod) + min;
    }
    // Place the min and max in two random spots
    int rand_loc1 = rand() % n;
    int rand_loc2 = -1;
    do {
      rand_loc2 = rand() % n;
    } while (rand_loc1 == rand_loc2);
    ints[rand_loc1] = min;
    ints[rand_loc2] = max;
  }
  return ints;
}

struct stats main_minsmax(char* file) {

  if (PROC_OUT){
    printf("I am the main process and my pid is %i\n", getpid());
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
  int stats[3];
  int pipes[2]; //read is pipe[0], write is pipe[1].
  if(pipe(pipes) < 0) { 
    perror("Error creating pipes");
    exit(1);
  }
  if (PROC_OUT) {
    printf("I am the main process and my pid is %i\n", getpid());
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

  int len = n / num_proc; // # of ints for all other processes
  int main_len = (n % num_proc) + len; // length of processing for main process

  int fpid = fork();

  if (fpid < 0) {
    //err
    perror("Error making child process");
  } else if (fpid == 0) {
    // child
    int* start = ints + main_len;
    recurse_minsmax_helper(start, num_proc - 1, len, pipes[1]);

  } else {
    // parent
    // Do calculations
    minsmax(ints, main_len, stats);
    // Get other data
    waitpid(fpid, NULL, 0);
    int odata[3];
    ssize_t bytes = read(pipes[0], &odata, sizeof(int)*3);
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

  }

  struct stats fin = { stats[0], stats[1], stats[2] };

  return fin;
}

void print_stats(struct stats stat) {
  printf("The min is %i\n", stat.min);
  printf("The max is %i\n", stat.max);
  printf("The sum is %i\n", stat.sum);
}

void recurse_minsmax_helper(int* data, int num_proc, int data_length, int wr_pipe) {

  if(num_proc == 0) { 
    write(wr_pipe, "\0", 1);
    close(wr_pipe);
    exit(0);
  }

  int stats[3];
  int pipes[2]; //read is pipe[0], write is pipe[1].
  if (PROC_OUT) {
    printf("I am a child process %i my parent is %i\n", getpid(), getppid());
  }

  if(pipe(pipes) < 0) { 
    perror("Error creating pipes");
    return;
  }
  int fpid = fork();

  if (fpid < 0) {
    //err
    perror("Error making child process");
  } else if (fpid == 0) {
    // child
    int* start = data + data_length;
    recurse_minsmax_helper(start, num_proc - 1, data_length, pipes[1]);

  } else {
    // parent
    // Do calculations
    minsmax(data, data_length, stats);
    // Get other data
    waitpid(fpid, NULL, 0);
    int odata[3];
    ssize_t bytes = read(pipes[0], (void*)odata, sizeof(int)*3);
    if( bytes == 1 ) {
      odata[0] = INT_MAX;
      odata[1] = INT_MIN;
      odata[2] = 0;
    } else if (bytes != sizeof(int)*3) {
      perror("Did not read the correct number of bytes from the pipe");
      return;
    }
    if(odata[0] < stats[0]) { stats[0] = odata[0]; } // Set new min if needed
    if(odata[1] > stats[1]) { stats[1] = odata[1]; } // Set new max if needed
    stats[2] += odata[2];
    bytes = write(wr_pipe, (void*)stats, sizeof(int)*3);
    close(wr_pipe);
    close(pipes[0]);
    close(pipes[1]);
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

  if (PROC_OUT) {
    printf("I am the main process and my pid is %i\n", getpid());
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
    } else if (fpid == 0) {
      if (PROC_OUT) {
        printf("I am a child process %i my parent is %i\n", getpid(), getppid());
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
  struct stats ret = {stats[0], stats[1], stats[2] };
  return ret;
}



