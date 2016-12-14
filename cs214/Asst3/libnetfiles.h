#ifndef __libnetfiles__
#define __libnetfiles__

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>

#define PORT (9797)

#define BUFF_SIZE 2048

#define NFS_UN 97 //Unrestricted mode
#define NFS_EX 98 //Exclusive write mode
#define NFS_TR 99 //Transactional mode

#define EHOSTNOTFOUND 1024

#define INVALID_FILE_MODE 700
#define INVALID_FLAG 800


typedef struct file_data {
    char* filename; // filename
    int sockfd; //client ID
    int file_fd; //file descriptor
    int file_connection; // Connection type (ext A)
    int flags; // O_RDONLY, O_WRONLY, or O_RDWR
    struct file_data* next;
} file_data;

typedef struct thread_rd {
    int sockfd;
    char* data;
    ssize_t data_size;
} thread_rd;

// Open a socket connection to a host with a port.
int socket_connect(char* host, int port);

// RETURN VALUE
// netopen() returns the new file descriptor, or -1 if an error occurred (in which case, errno is set
// appropriately)
int netopen(const char *pathname, int flags);


// RETURN VALUE
// Upon successful completion, netread() should return a non-negative integer indicating the
// number of bytes actually read. Otherwise, the function should return -1 and set errno to
// indicate the error.
ssize_t netread(int fildes, void *buf, size_t nbyte);


// RETURN VALUE
// Upon successful completion, netwrite() should return the number of bytes actually written to
// the file associated with fildes. This number should never be greater than nbyte. Otherwise, -1
// should be returned and errno set to indicate the error.
ssize_t netwrite(int fildes, const void *buf, size_t nbyte);


// RETURN VALUE
// netclose() returns zero on success. On error, -1 is returned, and errno is set appropriately.
int netclose(int fd);



// RETURN
// 0 on success, -1 on error and h_errnor set correctly
int netserverinit(char * hostname);
// netserverinit(char * hostname, int filemode) (for ext. A only)



union int_to_char {
    int a;
    char b[4];
};


void store_int(char* dest, int i);
int retr_int(const char* src);


void add_filedata(file_data** head, file_data* node);
file_data* remove_filedata(file_data** head, int fd_selector);
file_data* search_filedata(file_data** head, int fd_selector);
file_data* new_node(char* filename, int sockfd, int file_fd, int file_connection, int flags);
void free_filedata(file_data* node);

int max(int i1, int i2);
int min(int i1, int i2);

ssize_t read_func(int port, void *buf);

#endif



