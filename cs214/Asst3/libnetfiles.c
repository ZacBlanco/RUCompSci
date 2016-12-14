#include "libnetfiles.h"



char* host_server = NULL;


//Creates a socket and connects to the specified server
//Socket ready for reading and writing.
// Must check for errno afterwards
// Don't forget to close(sockfd) after.
int socket_connect(char* host) {
    int sockfd;
    struct sockaddr_in server;
    errno = 0;

    if (host == NULL) {
        errno = EHOSTNOTFOUND;
        return -1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Could not create socket.\n");
        return -1;
    }

    struct addrinfo* result;

    int herr = getaddrinfo(host, NULL, NULL, &result);

    if (herr < 0) {
        // Error on get addr info
        errno = EHOSTNOTFOUND;
        sockfd = -1;
    } else {

        struct sockaddr_in *addr;
        addr = (struct sockaddr_in *)result->ai_addr; 
        // printf("inet_ntoa(in_addr)sin = %s\n",inet_ntoa((struct in_addr)addr->sin_addr));
        bzero((char *) &server, sizeof(server));
        server.sin_addr = addr->sin_addr;
        server.sin_family = AF_INET;
        server.sin_port = htons( PORT );

        if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
            printf("Connection error\n");
            //errno is set by connect()
            sockfd = -1;
        } else {
            printf("Connection worked!\n");
        }

    }
    freeaddrinfo(result);
    return sockfd;
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
    int sfd = socket_connect(hostname);
    int ret = 0;
    printf("Socket desc: %i, errno: %i\n", sfd, errno);
    if ( errno != 0 ) { //errno is already set
        ret = -1;
    } else {

        char a = '0'; //Need to just send a '0' according to procol;
        write(sfd, &a, 1);
        printf("Send\n");
        char b[5];
        recv(sfd, &b, 5, 0);
        printf("recv[0], %i == %i?\n", b[0], '1');
        if (b[0] == '1') {
            ret = 0;
            host_server = hostname;
        } else {
            errno = retr_int(&( b[1] )); //gets error from msg
            ret = -1;
        }

        close(sfd);
    }
    return ret;
}


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

    char buffer[BUFF_SIZE], * data_received, num[10];
    int fd;
    int sockfd = socket_connect(host_server);

    if( !errno ) {
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
        fd = retr_int(data_received + 1);
        printf("fd: %i\n", fd);
        
        close(sockfd);
    } else {
        fd = -1;

    }
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

    int sockfd = socket_connect(host_server);
    char buffer[BUFF_SIZE], * data_received;

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


void add_filedata(file_data** head, file_data* node) {
    if (*head == NULL) {
        printf("set head as node\n");
        *head = node;
    } else {
        printf("didn't set head as node\n'");
        file_data* curr = *head;
        file_data* prev = *head;
        while(curr != NULL) {
            prev = curr;
            curr = curr->next;
        }
        prev->next = node;
        node->next = NULL;

    }
}

// Returns removed node on success, NULL if not found.
file_data* remove_filedata(file_data** head, int fd_selector) {
    file_data* curr = *head;
    file_data* prev = *head;

    if (curr->file_fd == fd_selector) {
        *head = (*head)->next;
        return curr;
    } 

    while(curr != NULL) {
        if(curr->file_fd == fd_selector) {
            break;
        }
        prev = curr;
        curr = curr->next; }

    if (curr == NULL) {
        return NULL;
    } else {
        prev->next = curr->next;
        return curr;
    }
}

file_data* search_filedata(file_data** head, int fd_selector) {
    file_data* curr = *head;
    while( curr != NULL ) {
        if (curr->file_fd == fd_selector) {
            return curr;
        }
    }
    return NULL;
}

file_data * new_node(char * filename, int sockfd, int file_fd, int file_connection, int flags) {

    file_data * node = malloc(sizeof(file_data));
    node -> filename = filename;
    node -> sockfd = sockfd;
    node -> file_fd = file_fd;
    node -> file_connection = file_connection;
    node -> flags = flags;

    return node;
}

void free_filedata(file_data* node) {
    free(node->filename);
    node->next = NULL;
    free(node);
}

