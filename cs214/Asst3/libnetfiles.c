#include "libnetfiles.h"

pthread_mutex_t lock;
char* host_server = NULL;
int global_filemode;

//Creates a socket and connects to the specified server
//Socket ready for reading and writing.
// Must check for errno afterwards
// Don't forget to close(sockfd) after.
int socket_connect(char* host, int port) {
    int sockfd;
    struct sockaddr_in server;
    errno = 0;
    if (host == NULL) {
        errno = EHOSTNOTFOUND;
        return -1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        //printf("Could not create socket.\n");
        return -1;
    }

    struct addrinfo* result;

    int herr = getaddrinfo(host, NULL, NULL, &result);
    if (herr < 0) {
        // Error on get addr info
        errno = EHOSTNOTFOUND;
        sockfd = -1;
    } else {
        errno = 0; // Set to 0 b/c implementation of getaddrinfo is weird on ilabs
        // For some reason even if it didn't fail errno was still being set.
        // If we made it up to this point anyways errno must have been 0 before
        // getaddrinfo()
        struct sockaddr_in *addr;
        addr = (struct sockaddr_in *)result->ai_addr; 
        // //printf("inet_ntoa(in_addr)sin = %s\n",inet_ntoa((struct in_addr)addr->sin_addr));
        bzero((char *) &server, sizeof(server));
        server.sin_addr = addr->sin_addr;
        server.sin_family = AF_INET;
        server.sin_port = htons( port );

        if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
            //printf("Connection error\n");
            //errno is set by connect()
            sockfd = -1;
        } else {
            //printf("Connection worked!\n");
        }

        if ( result != NULL) {
            freeaddrinfo(result);
        }
    }
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
int netserverinit(char * hostname, int filemode) {
    int sfd = socket_connect(hostname, PORT);
    int ret = 0;

    if (filemode == NFS_UN || filemode == NFS_EX || filemode == NFS_TR) {
        global_filemode = filemode;
        //printf("SET GLOBAL FILEMODE to %i\n", filemode);
    } else {
        errno = EINVAL;
        return -1;
    }
    
    //printf("Socket desc: %i, errno: %i\n", sfd, errno);
    if ( errno != 0 ) { //errno is already set
        ret = -1;
    } else {

        char a = '0'; //Need to just send a '0' according to protocol;
        write(sfd, &a, 1);
        //printf("Send\n");
        char b[5];
        recv(sfd, &b, 5, 0);
        //printf("recv[0], %i == %i?\n", b[0], '1');
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

    //printf("----------------------------------\n");
    //printf(" ~          NET OPEN             ~\n");
    //printf("----------------------------------\n");

    char buffer[BUFF_SIZE];
    int fd = 0;;
    int sockfd = socket_connect(host_server, PORT);

    if( !errno ) {
        buffer[0] = '1';
        store_int(buffer + 1, global_filemode);
        store_int(buffer + 5, flags);
        const char * filepath = pathname;
        memcpy(buffer + 9, filepath, strlen(filepath));
        if ( send(sockfd, buffer, strlen(filepath) + 9, 0) < 0) {
            //printf("Netopen failed on send.\n");
            fd = -1;
            close(sockfd);
            return fd;
        }

        if (recv(sockfd, buffer, BUFF_SIZE, 0) < 0) {
            //printf("Netopen failed on recv.\n");
            fd = -1;
        } else {
            //printf("Recv success.\n");
            fd = retr_int(buffer + 1);
        }
       
        if (fd == 1) {
            fd = -1;
        }

        close(sockfd);
    } else {
        fd = -1;
    }
    //printf("The returned fd is %i\n", fd);
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
    //printf("----------------------------------\n");
    //printf(" ~           NET READ            ~\n");
    //printf("----------------------------------\n");
    ssize_t ret = 0;
    char buffer[BUFF_SIZE];
    int sockfd = socket_connect(host_server, PORT); //implicitly sets errno=0

    if ( errno == 0 ) {
        buffer[0] = '3'; //3 for read.
        store_int(&( buffer[1] ), fildes);
        store_int(&( buffer[5] ), (int)nbyte);
        if ( write(sockfd, buffer, 9)  < 0) { //errno is set.
            ret = -1;
        }

        if (ret == -1 || recv(sockfd, buffer, BUFF_SIZE, 0) < 0) { //erno is set
            ret = -1;
        } else if (buffer[0] == '0') {
            // Failed to read
            errno = retr_int(&( buffer[1] ));
            ret = -1;
        } else if(buffer[0] == '1') {

            int sz = retr_int(&( buffer[1] ));
            memcpy(buf, &( buffer[5] ), sz);
            ret = (ssize_t)sz;

        } else if (buffer[0] == '2') {

            int nthreads = retr_int(&( buffer[1] ));
            int start_port = retr_int(&(  buffer[5] ));

            int pn = 0;
            for(pn = 0; pn < pn; pn++) {
                ssize_t r = read_func(start_port + pn, (buf + ret) );
                if (r == -1 && r + ret <= nbyte) {
                    errno = EIO;
                    break;
                }
                ret += r;
            }

        } else {
            errno = EBADMSG;
            ret = -1;
        }
        close(sockfd);
    } else { //fd is bad. //Errno already set
        ret = -1;
    }
    
    return ret;
}

ssize_t read_func(int port, void *buf) {
    ssize_t ret = 0;
    int data_size = BUFF_SIZE + 5;
    char buffer[data_size];
    int sockfd = socket_connect(host_server, port); //implicitly sets errno=0

    if ( errno == 0 ) {

        if (ret == -1 || recv(sockfd, buffer, data_size, 0) < 0) { //erno is set
            ret = -1;
        } else if (buffer[0] == '0') {
            // Failed to read
            errno = retr_int(&( buffer[1] ));
            ret = -1;
        } else if (buffer[0] == '1') {

            int sz = retr_int(&( buffer[1] ));
            memcpy(buf, &( buffer[5] ), sz);
            ret = (ssize_t)sz;
        } else {
            errno = EBADMSG;
            ret = -1;
        }
        close(sockfd);
    } else { //fd is bad. //Errno already set
        ret = -1;
    }
    
    return ret;
}

ssize_t write_func(int port, int orig_fd, const void *buf, size_t size) {
    ssize_t ret = 0;
    int data_size = BUFF_SIZE + 5;
    char buffer[data_size];
    int sockfd = socket_connect(host_server, port); //implicitly sets errno=0

    if ( errno == 0 ) {

        store_int(buffer, orig_fd);
        store_int(buffer + 4, (int)size);
        memcpy(buffer + 8, buf, size);

        if ( (ret = write(sockfd, buffer, size, 0) ) < 0) { //erno is set
            ret = -1;
        }

        if (ret < 0 || (ret = recv(sockfd, buffer, size, 0) ) < 0) { //erno is set
            ret = -1; //errno is set.
        } else {
            if (buffer[0] == '0') {
                errno = retr_int( &( buffer[1] ) );
                ret = -1;
            } else if (buffer[0] == '1') {
                ret = retr_int( &(buffer[1]) );
            }
        }

        close(sockfd);
    } else { //fd is bad. //Errno already set
        ret = -1;
    }

    return ret;
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
    
    //printf("----------------------------------\n");
    //printf(" ~           NET WRITE           ~\n");
    //printf("----------------------------------\n");
    ssize_t ret = 0;
    char buffer[BUFF_SIZE];
    int sockfd = socket_connect(host_server, PORT); //implicitly sets errno=0

    if ( errno == 0 ) {
        //printf("Error is none\n");

        if (nbyte < 2000) {

            buffer[0] = '4'; //4 for write.
            buffer[1] = '0'; // Are we greater than 2k? ==> No.
            store_int(&( buffer[2] ), fildes); //File descriptor
            store_int(&( buffer[6] ), (int)nbyte);
            memcpy(buffer + 10, buf, nbyte);
            if ( write(sockfd, buffer, 10 + nbyte)  < 0) { //errno is set.
                //printf("Error in write\n");
                ret = -1;
            }

            //printf("No error in write\n");

            if (ret == -1 || recv(sockfd, buffer, BUFF_SIZE, 0) < 0) { //erno is set
                //printf("Ret is equal -1");
                ret = -1;
            } else if (buffer[0] == '0') {
                //printf("Buffer[0] == 0\n");
                // Failed to read
                errno = retr_int(&( buffer[1] ));
                ret = -1;
            } else if (buffer[0] == '1') {
                //printf("Buffer[0] == 1\n");
                int sz = retr_int(&( buffer[1] ));
                ret = sz;

            } else {
                //printf("Error in receive\n");
                errno = EBADMSG;
                ret = -1;
            }
            close(sockfd);

        } else {

            // Size is over 2000 - special construction message.
            buffer[0] = '4'; //4 for write.
            store_int(&( buffer[1] ), '1'); // Are we greater than 2k? ==> No.
            store_int(&( buffer[5] ), fildes); //File descriptor
            store_int(&( buffer[9] ), (int)nbyte);
            if ( write(sockfd, buffer, 13)  < 0) { //errno is set.
                //printf("Error in write\n");
                ret = -1;
            }
            if (ret == -1 || recv(sockfd, buffer, BUFF_SIZE, 0) < 0) { //erno is set
                //printf("Ret is equal -1");
                ret = -1;
            } else if (buffer[0] == '0') {
                //Error on multiplexing
                errno = retr_int( buffer + 1 );
                ret = -1;
            } else if (buffer[0] == '1') {

                int nt = retr_int( &( buffer[5] ));
                int start_port = retr_int( &( buffer[9] ));
                ssize_t total_wr = 0;
                int wr_thsz = nt / nbyte;
                int rem = nbyte - (wr_thsz * nt);
                //Attempt to write data to every thread.
                //write_func(int port, int orig_fd, void *buf, size_t size)
                int i;
                for (i = 0; i < nt- 1; i++) {
                    ssize_t r = write_func(PORT + 1 + start_port, fildes, buf +(i*wr_thsz), wr_thsz);
                    if (r >= 0) {
                        total_wr += r;
                    }
                }
                ssize_t r = write_func(PORT + 1 + start_port, fildes, buf + (i*wr_thsz), rem);
                if (r >= 0) {
                    total_wr += r;
                }
                

            }

        }

    } else { //fd is bad. //Errno already set
        ret = -1;
    }
    
    return ret;
}


// RETURN VALUE
// netclose() returns zero on success. On error, -1 is returned, and errno is set appropriately.
int netclose(int fd) {

    //printf("----------------------------------\n");
    //printf(" ~          NET CLOSE            ~\n");
    //printf("----------------------------------\n");
    int ret = 0;
    int sockfd = socket_connect(host_server, PORT);
    char buffer[BUFF_SIZE];

    if ( errno == 0 ) {
        buffer[0] = '2';
        store_int(buffer + 1, fd);
        
        if ( send(sockfd, buffer, sizeof(char) + sizeof(int), 0) < 0) {
            //printf("Data sent failed.\n");
            ret = -1;
        }

        if ( ret == -1 || recv(sockfd, &buffer, sizeof(char) + sizeof(int), 0) < 0) {
            ret = -1; //errno is set already or gets set by recv.
        } else {
            //printf("Recv success.\n");
            ret = 0;
        }

        if (buffer[0] == '0') {
            int err = retr_int(buffer + 1);
            //printf("Error on netclose. %s\n", strerror(err));
            ret = -1;
        } else {
            //printf("Netclose success.\n");
            ret = 0;
        }
        
        close(sockfd);

    } else {

    }
    return 0;
}

// Pakcs int into a char array
void store_int(char* dest, int i) {
    union int_to_char t;
    t.a = i;
    dest[0] = t.b[0];
    dest[1] = t.b[1];
    dest[2] = t.b[2];
    dest[3] = t.b[3];
}


//unpacks int from a char array
int retr_int(const char* src) {
    union int_to_char t;
    t.b[0] = src[0];
    t.b[1] = src[1];
    t.b[2] = src[2];
    t.b[3] = src[3];
    return t.a;    
}


//Add node to end of list.
void add_filedata(file_data** head, file_data* node) {
    if (*head == NULL) {
        // //printf("set head as node\n");
        pthread_mutex_lock(&lock);
        *head = node;
        pthread_mutex_unlock(&lock);
    } else {
        // //printf("didn't set head as node\n'");
        file_data* curr = *head;
        file_data* prev = *head;
        while(curr != NULL) {
            prev = curr;
            curr = curr->next;
        }
        pthread_mutex_lock(&lock);
        prev->next = node;
        node->next = NULL;
        pthread_mutex_unlock(&lock);

    }
}

// Returns removed node on success, NULL if not found.
file_data* remove_filedata(file_data** head, int fd_selector) {
    file_data* curr = *head;
    file_data* prev = *head;

    if (curr->file_fd == fd_selector) {
        pthread_mutex_lock(&lock);
        *head = curr->next;
        pthread_mutex_unlock(&lock);
        return curr;
    } 

    while(curr != NULL) {
        if(curr->file_fd == fd_selector) {
            break;
        }
        prev = curr;
        curr = curr->next; }

    if (curr == NULL) {
        //printf("Couldn't find node to remove\n");
        return NULL;
    } else {
        //printf("Found node to be removed\n");
        pthread_mutex_lock(&lock);
        prev->next = curr->next;
        pthread_mutex_unlock(&lock);
        return curr;
    }
}

//Search for a node.
file_data* search_filedata(file_data** head, int fd_selector) {
    file_data* curr = *head;
    while( curr != NULL ) {
        //printf("Curr next: %p, fd = %i\n", curr->next, curr->file_fd);
        if (curr->file_fd == fd_selector) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

file_data* search_filedata_byname(file_data** head, char* name) {
    file_data* curr = *head;
    while( curr != NULL ) {
        //printf("Curent filename: %s\n", curr->filename);
        if (strcmp(curr->filename, name) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}


//creates a new node.
file_data * new_node(char * filename, int sockfd, int file_fd, int file_connection, int flags) {

    file_data * node = malloc(sizeof(file_data));
    node -> filename = filename;
    node -> sockfd = sockfd;
    node -> file_fd = file_fd;
    node -> file_connection = file_connection;
    node -> flags = flags;
    node -> next = NULL;
    //printf("NEW NODE WITH FILE DESC: %i\n", file_fd);
    return node;
}

void free_filedata(file_data* node) {
    free(node->filename);
    node->next = NULL;
    free(node);
}

int max(int i1, int i2) {
    if (i1 > i2) {
        return i1;
    } else {
        return i2;
    }
}

int min(int i1, int i2) {
    if (i1 > i2) {
        return i2;
    } else {
        return i1;
    }
}

