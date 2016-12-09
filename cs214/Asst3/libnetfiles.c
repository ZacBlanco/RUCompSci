#include "libnetfiles.h"




// RETURN VALUE
// netopen() returns the new file descriptor, or -1 if an error occurred (in which case, errno is set
// appropriately)
// ERRORS (check open manpage for definition)
// reqiured:
// EACCES
// EINTR
// EISDIR
// ENOENT
// EROFS
// optional (you may want/need)
// ENFILE
// EWOULDBLOCK
// EPERM
int netopen(const char *pathname, int flags) {
    return -1;
}



// RETURN VALUE
// Upon successful completion, netread() should return a non-negative integer indicating the
// number of bytes actually read. Otherwise, the function should return -1 and set errno to
// indicate the error.
// ERRORS (check manpage for definition)
// required:
// ETIMEDOUT
// EBADF
// ECONNRESET
ssize_t netread(int fildes, void *buf, size_t nbyte) {
    return -1;
}


// RETURN VALUE
// Upon successful completion, netwrite() should return the number of bytes actually written to
// the file associated with fildes. This number should never be greater than nbyte. Otherwise, -1
// should be returned and errno set to indicate the error.
// ERRORS
// required:
// EBADF
// ETIMEOUT
// ECONNRESET
ssize_t netwrite(int fildes, const void *buf, size_t nbyte) {
    return -1;
}


// RETURN VALUE
// netclose() returns zero on success. On error, -1 is returned, and errno is set appropriately.
int netclose(int fd) {
    return -1;
}




// netserverinit(char * hostname, int filemode) (for ext. A only)
// RETURN
// 0 on success, -1 on error and h_errnor set correctly
// ERRORS
// required:
// HOST_NOT_FOUND
// optional (if you do extension A)
// INVALID_FILE_MODE (be sure to include #define of this error code in your .h if you
// implement it)
int netserverinit(char * hostname) {
    return -1;
}


