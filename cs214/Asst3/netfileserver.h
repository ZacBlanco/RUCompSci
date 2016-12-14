#ifndef __netfileserver__
#define __netfileserver__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "libnetfiles.h"


void* client_handler(void* fd);
int process_msg(int sock, const char* buffer, ssize_t sz);

int close_op(int sock, const char* buffer, ssize_t sz);
int init_op(int sock, const char* buffer, ssize_t sz);
int open_op(int sock, const char* buffer, ssize_t sz);
int read_op(int sock, const char* buffer, ssize_t sz);
int write_op(int sock, const char* buffer, ssize_t sz);

int write_socket_err(int sock, int err);

#endif
