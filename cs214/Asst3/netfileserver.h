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

int get_max_multiplex(size_t data, int type);
int create_listen_socket(int port);
void handle_read(thread_rd* args);

int convert_fd(int fd);
int mode_allowed(int fmode, char* filename, int flags);

#endif

