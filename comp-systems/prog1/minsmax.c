#include "minsmax.h"

void main_minsmax(char* file) {
  printf("I am the main process and my pid is %i\n", getpid());
  int* ints = NULL;
  int n = readFile(file, &ints);

  if(n == -1) {
    perror("Bad File Input.");
    return;
  }
  int stats[3];
  minsmax(ints, n, stats);
  printf("The min is %i\n", stats[0]);
  printf("The max is %i\n", stats[1]);
  printf("The sum is %i\n", stats[2]);
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
  printf("minsmax: min, max, sum: %i, %i, %i\n", min, max, sum);
  results[0] = min;
  results[1] = max;
  results[2] = sum;
}

void main_recurse_minsmax(char* file, int num_proc) {
  int* ints = NULL;
  int stats[3];
  int pipes[2]; //read is pipe[0], write is pipe[1].
  if(pipe(pipes) < 0) { 
    perror("Error creating pipes");
    return;
  }
  
  printf("I am the main process and my pid is %i\n", getpid());
  if( num_proc <= 0 ) {
    perror("Number of processes must be > 1");
    return;
  }

  int n; // Number of ints read.
  if( (n = readFile(file, &ints)) == -1) {
    perror("Bad File Input.");
    return;
  }

  int len = n / num_proc; // # of ints for all other processes
  int main_len = (n % num_proc) + len; // length of processing for main process

  int fpid = fork();

  if (fpid < 0) {
    //err
    perror("Error making child process");
  } else if (fpid == 0) {
    // child
    int* start = ints + len;
    recurse_minsmax_helper(start, num_proc - 1, len, pipes[1]);

  } else {
    // parent
    // Do calculations
    minsmax(ints, main_len, stats);
    // Get other data
    waitpid(fpid, NULL, 0);
    int odata[3];
    ssize_t bytes = read(pipes[0], &odata, sizeof(int)*3);
    if (bytes != sizeof(int)*3) {
      perror("Did not read the correct number of bytes from the pipe");
      return;
    }
    if(odata[0] < stats[0]) { stats[0] = odata[0]; } // Set new min if needed
    if(odata[1] > stats[1]) { stats[1] = odata[1]; } // Set new max if needed
    stats[2] += odata[2];

  }

  printf("The min is %i\n", stats[0]);
  printf("The max is %i\n", stats[1]);
  printf("The sum is %i\n", stats[2]);
}

void recurse_minsmax_helper(int* data, int num_proc, int data_length, int wr_pipe) {

  if(num_proc == 0) { 
    write(wr_pipe, "\0", 1);
    close(wr_pipe);
    exit(0);
  }

  int stats[3];
  int pipes[2]; //read is pipe[0], write is pipe[1].
  printf("I am a child process %i my parent is %i\n", getpid(), getppid());
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




