#ifndef __libnetfiles__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>

// rw0 ===> r-0-0 = 4 ===> 0-w-0 = 2 ===> r-w-0 = 6
#define O_RDONLY 4
#define O_WRONLY 2
#define O_RDWR   6


#endif


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




