#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include "libnetfiles.h"

int main(){
  printf(" test 1 ======= 10pts===========\n");
  printf(" netserverinit(128.6.13.234)3pts\n");
  netserverinit("128.6.13.234", 2);
  getchar();/*
  printf(" netserverinit(derp.cs.rutgers.edu)2pts\n");
  netserverinit("derp.cs.rutgers.edu", 2);
  getchar();
  printf(" netserverinit(   )2pts\n");
  netserverinit("", 2);
  getchar();*/
  printf(" netserverinit(basic.cs.rutgers.edu)3pts\n");
  netserverinit("basic.cs.rutgers.edu", 2);
  getchar();

  printf(" test 2 ======= 10pts===========\n");
  printf("netopen\n", 2);
  int netfd = netopen("./file.txt", O_RDWR);
  printf("netfd is %d\n", netfd);
  getchar();

  printf(" test 3 ======= 20pts===========\n");

  char buf[200]= "This is a test! From test!";
  netwrite(netfd, buf, 25);
  printf("Write is buf = %s\n", buf);
  getchar();

  bzero(buf,200);
  netread(netfd, buf, 25);
  printf("Read is buf = %s\n", buf);
  getchar();

  return 0;
}
