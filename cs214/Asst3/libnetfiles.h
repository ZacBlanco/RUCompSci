#ifndef __libnetfiles__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>


#define BUFF_SIZE 512

#define NFS_UN 97 //Unrestricted mode
#define NFS_EX 98 //Exclusive write mode
#define NFS_TR 99 //Transactional mode

#define INVALID_FILE_MODE 700
#define INVALID_FLAG 800

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

#endif



