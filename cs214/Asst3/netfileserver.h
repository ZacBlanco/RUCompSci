#ifndef __netfileserver__

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

#define PORT (9797)

typedef struct file_data {
    char* filename;
    int sockfd;
    int file_fd;
    int file_connection;
    int flags;
    struct file_data* next;
} file_data;

void* client_handler(void* fd);
int process_msg(int sock, const char* buffer, ssize_t sz);


void add_filedata(file_data* head, file_data* node);
file_data* remove_filedata(file_data* head, int fd_selector);
file_data* search_filedata(file_data* head, int fd_selector);
file_data* new_node(char* filename, int sockfd, int file_fd, int file_connection, int flags);
void free_filedata(file_data* node);

int close_op(int sock, const char* buffer, ssize_t sz);
int init_op(int sock, const char* buffer, ssize_t sz);
int open_op(int sock, const char* buffer, ssize_t sz);

int write_socket_err(int sock, int err);

#endif

