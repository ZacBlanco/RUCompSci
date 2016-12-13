#ifndef __netfileserver__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "libnetfiles.h"

#define PORT (9797)

#endif

typedef struct {
    char* filename;
    int fd;
    int file_mode;
    int conn_mode;
    struct filenode* next;
} filenode;

union int_to_char {
    int a;
    char b[4];
};


void* client_handler(void* fd);
int process_msg(int sock, const char* buffer, ssize_t sz);
void store_int(char* dest, int i);
int retr_int(char* src);

void add_filenode(filenode* head, filenode* node);
filenode* remove_filenode(filenode* head, int fd_selector);
void free_filenode(filenode* node);
