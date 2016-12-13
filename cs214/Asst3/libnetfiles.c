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

    printf("----------------------------------\n");
    printf(" ~          NET OPEN             ~\n");
    printf("----------------------------------\n");


    int sockfd;
    struct sockaddr_in server;
    char buffer[BUFF_SIZE], * data_received, num[10];
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        printf("Could not create socket.\n");
        return -1;
    }

    bzero((char *) &server, sizeof(server));
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 9797 );

    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Connection error\n");
        return -1;
    } else {
        printf("Connection worked!\n");
    }

    buffer[0] = '1';
    store_int(buffer + 1, 97);
    store_int(buffer + 5, O_RDONLY);
    char * filepath = "./test.txt";
    memcpy(buffer + 9, filepath, strlen(filepath));
    if ( send(sockfd, buffer, strlen(filepath) + 9, 0) < 0) {
        printf("Data sent failed.\n");
    } else {
        printf("Data sent success.\n");
    }

    if ( recv(sockfd, data_received, strlen(data_received), 0) < 0) {
        printf("Recv failed.\n");
    } else {
        printf("Recv success.\n");
    }
    int fd = retr_int(data_received + 1);
    printf("fd: %i\n", fd);
    
    close(sockfd);
    return fd;

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

    printf("----------------------------------\n");
    printf(" ~          NET CLOSE            ~\n");
    printf("----------------------------------\n");

    int sockfd;
    struct sockaddr_in server;
    char buffer[BUFF_SIZE], * data_received;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        printf("Could not create socket.\n");
        return -1;
    }

    bzero((char *) &server, sizeof(server));
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 9797 );

    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Connection error\n");
        return -1;
    } else {
        printf("Connection worked!\n");
    }

    buffer[0] = '2';
    store_int(buffer + 1, fd);
    if ( send(sockfd, buffer, sizeof(char) + sizeof(int), 0) < 0) {
        printf("Data sent failed.\n");
    } else {
        printf("Data sent success.\n");
    }

    if ( recv(sockfd, data_received, strlen(data_received), 0) < 0) {
        printf("Recv failed.\n");
    } else {
        printf("Recv success.\n");
    }
    if (data_received[0] == '0') {
        int err = retr_int(data_received + 1);
        printf("Error on netclose. %s\n", strerror(err));
    } else {
        printf("Netclose success.\n");
        return 1;
    }
    
    close(sockfd);
    return 0;
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

void store_int(char* dest, int i) {
    union int_to_char t;
    t.a = i;
    dest[0] = t.b[0];
    dest[1] = t.b[1];
    dest[2] = t.b[2];
    dest[3] = t.b[3];
}

int retr_int(const char* src) {
    union int_to_char t;
    t.b[0] = src[0];
    t.b[1] = src[1];
    t.b[2] = src[2];
    t.b[3] = src[3];
    return t.a;    
}


