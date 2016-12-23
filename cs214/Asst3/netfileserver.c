#include "netfileserver.h"

file_data* head = NULL;
pthread_t multiplex_threads[10];
static int thread_taken[10];
pthread_mutex_t multiplex_lock;

int main(int argc, char** argv) {



    printf("I am the fileserver\n");
    int sfd = socket(AF_INET, SOCK_STREAM, 0); // socket fd
    // set SO_REUSEADDR on a socket to true ==> 1:
    int optval = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    if (sfd < 0) {
        perror("Server failed to start. Error on socket()");
        exit(1);
    }

    //Code borrowed from http://www.binarytides.com/socket-programming-c-linux-tutorial/
    struct sockaddr_in server_info;
    server_info.sin_family = AF_INET;
    server_info.sin_addr.s_addr = INADDR_ANY;
    server_info.sin_port = htons( PORT );

    if (bind(sfd , (struct sockaddr *)&server_info , sizeof(struct sockaddr_in)) < 0) {
        perror("Could not bind() socket");
        exit(1);
    }
    //Code borrowed from http://www.binarytides.com/socket-programming-c-linux-tutorial/

    //We're bound now and good to go
    listen(sfd, 5);
    printf("listening for new connections...\n");

    
    pthread_t pre_thread;
    void * (*worker)(void *);
    worker = (void *)(&client_handler);
    size_t c = sizeof(struct sockaddr_in);
    struct sockaddr_in client;
    int conn; 
    while (1) {
        conn = accept(sfd, (struct sockaddr *)&client, (socklen_t*)&c);
        if (conn < 0) {
            perror("Failed on accept()");
            continue;
        } else {
            printf("Conneción Accepted\n");

            //Handle the clients
            int* fd = malloc(sizeof(int)); //FREE THIS IN WORKER
            *fd = conn; 
            pthread_create(&pre_thread, NULL, client_handler,(void *) fd);
            if ( pthread_detach(pre_thread)  < 0) {
                perror("Issue detaching worker thread");
            }

        } 
    }

    return 0;
}


/* 
====================
PROTOCOL DESCRIPTION
====================

First char (byte) always describes operation.

    '0' - init
    '1' - open
    '2' - close
    '3' - read
    '4' - write



    ~~~~~~~~ SERVER SIDE ~~~~~~~~ 
    ~~~~~~~~ SERVER SIDE ~~~~~~~~ 
 
    ------ '0' - INIT Operation ------ 

    - If init is received then we write a simple response with suc=1

    +---------+-----------------+-----------------------------+
    | op. (1) | filemode (4)    |                             |
    +---------+-----------------+-----------------------------+

    - The response to an INIT message is a single byte which is
    set to '1' to indicate that the server exists. Otherwise an
    error message is returned

    Expected Response on Success
    +-----------+---------------------------------------------+
    | suc=1 (1) |                                             |
    +-----------+---------------------------------------------+
    Expected Response on Failure
    +-----------+-----------+----------------+----------------+
    | suc=0 (1) | errno (4) | errm messg (n) |                |
    +-----------+-----------+----------------+----------------+


    ------ '1' - Open Operation ------ 

    - On an open operation we have a first bit of '1'. After
    which we care about 2 arguments. The filename, and the
    flags.

    Expected Message
    +---------+-----------+--------------+--------------------+
    | op. (1) | flags (4) | fmode (4)    | filepath (n)       |
    +---------+-----------+--------------+--------------------+

    - Responses differ in type based on failure/success

    Expected Response on Success
    +-----------+----------------+----------------------------+
    | suc=1 (1) | descriptor (8) |                            |
    +-----------+----------------+----------------------------+
    Expected Response on Failure
    +-----------+-----------+----------------+----------------+
    | suc=0 (1) | errno (4) | errm messg (n) |                |
    +-----------+-----------+----------------+----------------+


    ------ '2' - Close Operation ------

    - On a close operation we have a first bit of 2. After
    which we care about 1 argument - The file descriptor.

    Expected Message
    +---------+----------------+------------------------------+
    | op. (1) | descriptor (4) |                              |
    +---------+----------------+------------------------------+

    - Responses differ in type based on failure/success

    Expected Response on Success
    +-----------+---------------------------------------------+
    | suc=1 (1) |                                             |
    +-----------+---------------------------------------------+
    Expected Response on Failure
    +-----------+-----------+----------------+----------------+
    | suc=0 (1) | errno (4) | errm messg (n) |                |
    +-----------+-----------+----------------+----------------+

    ------ '3' - Read Operation ------

     - On a read operation we have a first bit of '3'. After
    which we care about 1 argument - The file descriptor.

    Expected Message
    +---------+----------------+-----------+------------------+
    | op. (1) | descriptor (4) | size (4)  |                  |
    +---------+----------------+-----------+------------------+

    - Responses differ in type based on failure/success
    - Response will vary based on number of bytes requested

    Expected Response on handshake
    +-----------+-------------+----------------+--------------+
    | suc=1 (1) | nthread (4) | start_port (n) |              |
    +-----------+-------------+----------------+--------------+
    Expected Response on Failure
    +-----------+-----------+----------------+----------------+
    | suc=0 (1) | errno (4) | errm messg (n) |                |
    +-----------+-----------+----------------+----------------+

    After the handshake there shall be `nthread` number of threads open
    to listen on.

    Expected Response on Success
    +-----------+---------------------+-----------------------+
    | suc=1 (1) | size (4) | data (n) |                       |
    +-----------+---------------------+-----------------------+
    Expected Response on Failure
    +-----------+-----------+----------------+----------------+
    | suc=0 (1) | errno (4) | errm messg (n) |                |
    +-----------+-----------+----------------+----------------+


    ------ '4' - Write Operation ------ 

    - On a write operation we have a first bit of '4'. After
    which we care about 1 argument - The file descriptor. write_op

    Expected Message (size < 2k)
    +---------+-----------+----------------+----------+---------+
    | op. (1) | sgt2k (1) | descriptor (4) | size (4) | data(n) |
    +---------+-----------+----------------+----------+---------+
    Expected Message (size > 2k)
    +---------+-----------+----------------+----------+---------+
    | op. (1) | sgt2k (1) | descriptor (4) | size (4) |         |
    +---------+-----------+----------------+----------+---------+

    - Responses differ in type based on failure/success

    Expected Response on Success (for less than 2k)
    +-----------+---------+-----------------------------------+
    | suc=1 (1) | size(4) |                                   |
    +-----------+---------+-----------------------------------+
    Expected Response on Failure
    +-----------+-----------+----------------+----------------+
    | suc=0 (1) | errno (4) | errm messg (n) |                |
    +-----------+-----------+----------------+----------------+

/*
    Expected Response on Success (for gt 2k)
    +----------+-------------+----------------+---------------+
    | suc=1 (1) | nthread (4) | start_port (n) |              |
    +-----------+-------------+----------------+--------------+
    Expected Response on Failure
    +-----------+-----------+----------------+----------------+
    | suc=0 (1) | errno (4) | errm messg (n) |                |
    +-----------+-----------+----------------+----------------+

    Client will then send data to write. in the form
    Expected Response on Success (for gt 2k)
    +-------------+------------+----------+--------------------+
    | orig_fd (4) | data_sz(4) | data (n) |                    |
    +-------------+------------+----------+--------------------+

    The thread should then redpond with
    +-----------+---------+-----------------------------------+
    | suc=1 (1) | size(4) |                                   |
    +-----------+---------+-----------------------------------+
    or 
    +-----------+-----------+----------------+----------------+
    | suc=0 (1) | errno (4) | errm messg (n) |                |
    +-----------+-----------+----------------+----------------+



*/


/* Worker - void* fd is the socket file descriptor */
void* client_handler(void* fd) {
    char* buff[BUFF_SIZE];
    int sock = *(int*)fd;
    int run_thread = 1;
    while (run_thread) {
        ssize_t bytes = recv(sock, buff, BUFF_SIZE, 0);

        if (bytes == 0) {
            printf("User closed connection\n");
            break; //connection closed.
        } else if (bytes == -1) {
            perror("Error on recv() of bytes");
        } else {
            // We got a good message
            printf("Got: %s\n", (char*)buff);
            if (process_msg(sock, (char *)buff, bytes) < 0) {
                break;
            }   
        }
    }
    
    close(sock);
    printf("Closed connection.\n");
    free(fd);
    pthread_exit(NULL);
}


// Processes the message;
// Returns 0 if we should continue to listen for messages.
// Returns -1 to close the connection.
// ARGS:
//      int sock: The socket fd
//      const char* buffer: Input
//      ssize_t sz: The max amount of bytes to read from.
int process_msg(int sock, const char* buffer, ssize_t sz) {
    // First determine the operation.
    int return_code = 0;
    switch(buffer[0]) {
        case '0':
            return_code = init_op(sock, buffer, sz);
            break;

        case '1':
            return_code = open_op(sock, buffer, sz);
            break;

        case '2':
            return_code = close_op(sock, buffer, sz);
            break;

        case '3':
            return_code = read_op(sock, buffer, sz);
            break;

        case '4':
            return_code = write_op(sock, buffer, sz);
            break;

        default:
            printf("Bad message\n");
            break;

    }
    printf("~~~~~~~ Operation END ~~~~~~~\n");

    return return_code;
}


// Returns the number of bytes written back to the client.
int close_op(int sock, const char* buffer, ssize_t sz) {
    
    printf("--------------------------\n");
    printf("Close operation received.\n");
    printf("--------------------------\n");

    int badf = 0; // true if we need to send an error; 
    int sz_wr = 0;
    if (sz < 5) {
        // Message did not contain enough bytes to process
        printf("Size less than 5: %d\n", (int)sz);
        badf = 1;
    } else {
        int fd = convert_fd(retr_int( buffer + 1 ));
        printf("fd received for close: %i\n", convert_fd(fd));
        file_data* closen = search_filedata(&head, fd);
        printf("Search completed.\n");
        if (closen == NULL) {
            //File did not exist.
            printf("File did not exist.\n");
            badf = 1;
        } else {
            printf("Found filein list.\n");
            remove_filedata(&head, fd);
            free_filedata(closen);
            close(fd);
        }
    }

    // Write depending on whether file descriptor was bad.
    if (badf) {
        char* mg = "Bad file descriptor";
        int len = 5 + strlen(mg);
        char* msg = malloc(sizeof(char) * len);
        msg[0] = '0';
        store_int(&(msg[1]), EBADF);
        strcpy(&(msg[5]), mg);
        sz_wr = write(sock, msg, len);
        free(msg);
    } else {
        printf("Success\n");
        char msg[5];
        store_int(&(msg[1]), 1);
        sz_wr = write(sock, &msg, 1);
    }

    return sz_wr;

}


int init_op(int sock, const char* buffer, ssize_t sz) {
    printf("--------------------------\n");
    printf("Init Operation received\n");
    printf("--------------------------\n");
    int wrsz = 0;

    //Code for ext A init.
    // if (sz < 5) {
    //     wrsz = write_socket_err(sock, EINVAL);    
    // } else {
    //     int mode = retr_int(buffer + 1);
    //     char a;
    //     if (mode == NFS_EX || mode == NFS_TR || mode == NFS_UN) {
    //         a = '1';
    //         wrsz = write(sock, &a, 1);
    //     } else {
    //         a = '0';
    //         wrsz = write(sock, &a, 1);
    //     }   
    // }

    char a;
    a = '1';
    wrsz = write(sock, &a, 1);
    
    return wrsz;
}

//Return number of bytes written to client.
int open_op(int sock, const char* buffer, ssize_t sz) {

    printf("--------------------------\n");
    printf("Open Operation Received\n");
    printf("--------------------------\n");
    int wrsz = 0; //write size;
    int fd = 0;
    int err = 0;
    if (sz < 9) {
        wrsz = write_socket_err(sock, EINVAL);
    } else {
        int fmode = retr_int(buffer + 1);
        int flags = retr_int(buffer + 5);
        char* filepath = malloc(sizeof(char) * (sz - 9 + 1)); //sz-9 is the number of chars for filename + null
        filepath[0] = '\0';
        strncpy(filepath, buffer + 9, sz-9);
        filepath[sz-9] = '\0';


        printf("filepath: %s\n", filepath);
        // printf("buffer: %s fmode: %i flags: %i filepath: %s\n", buffer, fmode, flags, filepath);

        int fd = 0;
        int err = 0;
        
        if ( !err ) {
            err = check_flags(sock, flags);
        }

        err = !(mode_allowed(fmode, filepath, flags));

        if ( err == 1) { 
            fd = -1;
        }

            // If there was no error on filemode or flags then we're ok.
            if ( err != 1  && fd != -1) {
                switch (flags) {
                    case O_RDWR:
                        fd = open(filepath, O_RDWR); 
                        break;
                    case O_RDONLY:
                        fd = open(filepath, O_RDONLY);
                        break;
                    case O_WRONLY:
                        fd = open(filepath, O_WRONLY);
                        break;
                }
            }

        if ( fd < 0) { // open failed - get error
            perror("Got FD as < 0");
            printf("Bad File. Errno - %s\n", strerror(errno));
            wrsz = write_socket_err(sock, errno);
        } else {
            file_data * node = new_node(filepath, sock, fd, fmode, flags);
            add_filedata(&head, node);
            char a[5];
            a[0] = '1';
            printf("Returning fd from open_op %i\n", convert_fd(fd));
            store_int(&( a[1] ), convert_fd(fd));
            wrsz = send(sock, &a, 5, 0);
            // printf("fd: %i\n", fd);
        }
    }
    return wrsz;
}

//
int mode_allowed(int fmode, char* filename, int flags) {
    file_data* node = search_filedata_byname(&head, filename);
    int ret = 1;

    if(node == NULL) {
        return 1;
    }
    printf("Checking if mode allowed: %i, on file %s, with flags %i\n", fmode, filename, flags);
    switch(fmode) {
        case NFS_UN:
            printf("mode is UNRESTRICTED and found a node.\n");
            printf("node fileconnection : %i\n", node->file_connection);
            if(node->file_connection == NFS_TR) {
                ret = 0;
            } else if(node->file_connection == NFS_EX && node->flags == O_RDONLY) {
                file_data* n = node->next;
                while (n != NULL) {
                    if (  (n->flags == O_WRONLY || node->flags == O_RDWR) && (flags == O_WRONLY || flags == O_RDWR)) {
                        ret = 0;
                        break;
                        n = search_filedata_byname(&(n->next), filename);
                    }
                }
            } else if( node->file_connection == NFS_EX && node->flags == O_RDWR && (flags == O_WRONLY)) {
                ret = 0;
            } else if( node->file_connection == NFS_EX && node->flags == O_RDWR && flags == O_RDWR) {
                ret = 0;
            } else if (node->file_connection == NFS_EX && node->flags == O_RDWR && (flags == O_WRONLY || flags == O_RDWR)) {
                ret = 0;
            }

            break;
        case NFS_EX:
            if(node->file_connection == NFS_TR) {
                ret = 0;
            } else if(node->file_connection == NFS_EX && node->flags == O_RDONLY) {
                file_data* n = node->next;
                while (n != NULL) {
                    if (  (n->flags == O_WRONLY || node->flags == O_RDWR) && (flags == O_WRONLY || flags == O_RDWR)) {
                        ret = 0;
                        break;
                    }
                    n = search_filedata_byname(&(n->next), filename);
                }
            } else if( node->file_connection == NFS_EX && node->flags == O_RDWR && flags == O_WRONLY) {
                ret = 0;
            } else if( node->file_connection == NFS_EX && node->flags == O_RDWR && flags == O_RDWR) {
                ret = 0;
            } else if (node->file_connection == NFS_UN && (node->flags == O_RDWR || node->flags == O_WRONLY ) && (flags == O_RDWR) ) {
                printf("Inside what shoudl catch\n");
                ret = 0;
            } else if (node->file_connection == NFS_UN && (node->flags == O_RDWR || node->flags == O_WRONLY ) && (flags == O_WRONLY) ) {
                printf("Inside what shoudl catch 2\n");
                ret = 0;
            }

            break;
        case NFS_TR:
            if(node->file_connection == NFS_TR) {
                ret = 0;
            } else if(node->file_connection == NFS_EX) {
                ret = 0;
            } else if( node->file_connection == NFS_UN) {
                ret = 0;
            } 
            break;

        default:
            ret = 0;
            errno = EBADMSG;
            break;
    }

    if (ret == 0) {
        errno = EPERM;
    }
    printf("RETURN FROM MODE ALLOWED: %i\n", ret);
    return ret;

}

int read_op(int sock, const char* buffer, ssize_t sz){
    printf("--------------------------\n");
    printf("Read Operation Received\n");
    printf("--------------------------\n");
    int wrsz = 0;
    file_data* entry = NULL;
    if (sz < 9) {
        wrsz = write_socket_err(sock, EINVAL);
        printf("Read Size not great enough.\n");    
    } else {

        int ffd = convert_fd(retr_int(buffer + 1));
        entry = search_filedata(&head, ffd);
        if (entry != NULL) {
            printf("Entry flags: %i\n", entry->flags);
        }
        
    }

    if (entry == NULL) {
        wrsz = write_socket_err(sock, EBADF);
        printf("No entry\n");
    } else if ( entry->flags == O_RDONLY || entry->flags == O_RDWR ) {

        int rd_sz = retr_int(buffer + 5);
        printf("Size to read is %i\n", rd_sz);

        if (rd_sz > 2000) {
            printf("Multiplexing the read\n");

            pthread_mutex_lock(&multiplex_lock);
<<<<<<< HEAD
            int nt = get_max_multiplex(rd_sz, 0, thread_taken); // Num threads
            int st = get_max_multiplex(rd_sz, 1, thread_taken); // Index where we can start from.
            printf("# of threads: %i index start: %i\n", nt, st);
=======
            int nt = get_max_multiplex(rd_sz, 0); // Num threads
            int st = get_max_multiplex(rd_sz, 1); // Index where we can start from.
>>>>>>> 254fff55de1e78eae5bdbeb74389557507fbc432
            if  (nt <= 0) {
                wrsz = write_socket_err(sock, EFBIG);
                return wrsz;    
            }
            int i;
            for (i = 0; i < nt; i++) {
                thread_taken[i + st] = 1;
            }
            pthread_mutex_unlock(&multiplex_lock);

            // we now have exclusive access to multiplex threads st through (st + nt)
            int* socks = malloc(sizeof(int)*nt);
            errno = 0;
            for(i = 0; i < nt; i++) {
                int s = create_listen_socket( PORT + i + 1 );
                if (s == -1) { // uh oh -- error close all sockets.
                    printf("Error closing all sockets.\n");
                    i--;
                    while (i >= 0) {
                        close(socks[i]);
                        i--; 
                    }
                    break;
                }
                socks[i] = s;
                printf("Sock[%i]: %i\n", i, socks[i]);
            }

            printf("Starting point.\n");
                    printf("Sock[1]: %i\n", socks[1]);

            if ( errno != 0 ) {
                //Shouldn't create threads -sockets are closed
                printf("Errno != 0\n");
                wrsz = write_socket_err(sock, errno);
            } else {
                //Go on ahead and create the threads.
                printf("Errno == 0\n");
                size_t data_per_thread = sz / nt;
                size_t rem = sz - (data_per_thread * nt); 

                void * (*worker)(void *);
                worker = (void *)(&handle_read);
                printf("About to run threads.\n");
                    printf("Sock[1]: %i\n", socks[1]);
                for(i = 0; i < nt - 1; i++) {
                    printf("Sock[1]: %i\n", socks[1]);
                    char* a = malloc(sizeof(char)*(data_per_thread + 5));
                    thread_rd* s = malloc(sizeof(thread_rd));
                    ssize_t r = read(entry->file_fd, a+5, data_per_thread);
                    if (r < 0) {
                        printf("Checking.\n");
                        a[0] = '0';
                        store_int(a +1, errno);
                        s -> sockfd = socks[i];
                        s -> data = a;
                    } else {
                    printf("Sock[1]: %i\n", socks[1]);
                        a[0] = '1';
                        store_int(a + 1, data_per_thread);
                        printf("Socks: %i\n", socks[i]);
                        s -> sockfd = socks[i];
                        printf("Socks after set: %i\n", s -> sockfd);
                        s -> data = a;
                        s -> data_size = r;
                        //Now send off to the threads.
                    }
                    
                    printf("Sock[1]: %i\n", socks[1]);
                    pthread_create(&(multiplex_threads[i]), NULL, worker, s);
                }
                    printf("Sock[1]: %i\n", socks[1]);
                    char* a = malloc(sizeof(char)*(rem + 5));
                    thread_rd* s = malloc(sizeof(thread_rd));
                    printf("Sock[1]: %i\n", socks[1]);
                    ssize_t r = read(entry->file_fd, a+5, rem);
                    printf("Sock[1]: %i\n", socks[1]);
                    if (r < 0) {
                        a[0] = '0';
                        store_int(a + 1, errno);
                        printf("Socks: %i\n", socks[i + 1]);
                        s -> sockfd = socks[i + 1];
                        printf("Socks after set: %i\n", socks[i + 1]);
                        s -> data = a;
                    } else {
                        a[0] = '1';
                        store_int(a + 1, data_per_thread);
                        printf("i: %i\n", i);
                        printf("Socks: %i\n", socks[i]);
                        s -> sockfd = socks[i];
                        printf("Socks after set: %i\n", s -> sockfd);
                        s -> data = a;
                        s -> data_size = r;
                        //Now send off to the threads.
                    }
<<<<<<< HEAD
                    pthread_create(&(multiplex_threads[i + 1]), NULL, worker, s);

                // WRITE BACK HERE
                wrsz = write(sock, data, datasz);
                printf("Threads running\n");
            }



=======
                    pthread_create(&(multiplex_threads[i]), NULL, worker, s);
                    free(socks);
                    for(i = 0; i < nt; i++) {
                        pthread_join(multiplex_threads[i], NULL);
                        pthread_mutex_lock(&multiplex_lock);
                        thread_taken[i + st] = 0;
                        pthread_mutex_unlock(&multiplex_lock);
                    }
                }
>>>>>>> 254fff55de1e78eae5bdbeb74389557507fbc432
        } else {
            printf("Not bigger than 3500.\n");

            int datasz = rd_sz + 5; // 1 for success and 4 for bytes read.
            // Entry not null -  read and return data        
            char* data = malloc(sizeof(char)*(datasz));
            ssize_t bts_read = read(entry->file_fd, data + 5, rd_sz);
            printf("read bytes %zd\n", bts_read);
            if(bts_read < 0) {
                wrsz = write_socket_err(sock, errno);
            } else {
                data[0] = '1';
                store_int(data + 1, bts_read);
                wrsz = write(sock, data, datasz);

                if (wrsz < 0) {
                    wrsz = write_socket_err(sock, errno);
                }
            }
            free(data);

        }

    } else { //File descriptor was write only
        printf("File descriptor was write only.\n");
        wrsz = write_socket_err(sock, EPERM);
    }
    return wrsz;
}


void handle_read(thread_rd* args) {
    size_t c = sizeof(struct sockaddr_in);
    struct sockaddr_in client;
    int conn; 
    conn = accept(args->sockfd, (struct sockaddr *)&client, (socklen_t*)&c);
    if (conn < 0) {
        printf("Args -> sockfd: %i\n", args -> sockfd);
        perror("Failed on accept()");
    } else {
        printf("Conneción Accepted\n");
        write(args->sockfd, args->data, args->data_size);
    }
    free(args->data);
    free(args);
    
    
}

//Creates a bunch of new sockets listening on (port)
int create_listen_socket(int port) {
    int sfd = socket(AF_INET, SOCK_STREAM, 0); // socket fd
    // set SO_REUSEADDR on a socket to true ==> 1:
    int optval = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    if (sfd < 0) {
        perror("Could not open multiplex port. Error on socket()");
        return -1;
    }

    //Code borrowed from http://www.binarytides.com/socket-programming-c-linux-tutorial/
    struct sockaddr_in server_info;
    server_info.sin_family = AF_INET;
    server_info.sin_addr.s_addr = INADDR_ANY;
    server_info.sin_port = htons( port );

    if (bind(sfd , (struct sockaddr *)&server_info , sizeof(struct sockaddr_in)) < 0) {
        perror("Could not bind() socket");
        return -1;
    }
    //Code borrowed from http://www.binarytides.com/socket-programming-c-linux-tutorial/

    //We're bound now and good to go
    if ( listen(sfd, 5) == -1) {
        perror("Error listen() on multiplex port");
        return -1;
    } else {
        return sfd;
    }
}

int write_op(int sock, const char* buffer, ssize_t sz) {
    printf("--------------------------\n");
    printf("Write Operation Received\n");
    printf("--------------------------\n");
    int wrsz = 0;
    file_data* entry = NULL;
    if (sz < 10) {
        wrsz = write_socket_err(sock, EBADE);    
    } else {

        int ffd = convert_fd(retr_int(buffer + 2));
        entry = search_filedata(&head, ffd);
        
    }
    printf("Entry is: %p.\n", entry);

    if (entry == NULL) {
        wrsz = write_socket_err(sock, EINVAL);
    } else if(entry->flags == O_RDWR || entry->flags == O_WRONLY){
        
        int greater_than_2k = buffer[1];

        if (greater_than_2k == '0') {
            int write_amount = retr_int(buffer + 6);
            ssize_t bts_written = write(entry->file_fd, buffer + 10, write_amount);

            if(bts_written < 0) {
                wrsz = write_socket_err(sock, errno);
            } else {
                char a[5];
                a[0] = '1';
                store_int(a + 1, bts_written);
                wrsz = write(sock, a, write_amount);

                if (wrsz < 0) {
                    wrsz = write_socket_err(sock, errno);
                }
            }

        } else if (greater_than_2k == '1') {
            int write_amount = retr_int(buffer + 6);
            // We need to create the threads here.
            pthread_mutex_lock(&multiplex_lock);
            int nt = get_max_multiplex(write_amount, 0); // Num threads
            int st = get_max_multiplex(write_amount, 1); // Index where we can start from.
            if  (nt <= 0) {
                wrsz = write_socket_err(sock, EFBIG);
                return wrsz;    
            }
            int i;
            for (i = 0; i < nt; i++) {
                thread_taken[i + st] = 1;
            }
            pthread_mutex_unlock(&multiplex_lock);
            size_t wr_thsz = write_amount / nt;
            size_t rem = write_amount % nt;
            // So now we have # threads and the start port.
            //Need nt new file descriptors which write to 
            //different locations in the file
            //meaning we dup the current file descriptor.
            int* new_fds = malloc(sizeof(int)*nt);
            off_t pos = lseek(entry->file_fd, 0, SEEK_CUR);
            for(i = 0; i < nt-1; i++){
                new_fds[i] = open(entry->filename, entry->flags);
                lseek(new_fds[i], pos + (i*wr_thsz), SEEK_CUR); // Mov to current file position
            }
            new_fds[i] = open(entry->filename, entry->flags);
            lseek(new_fds[i], pos + (i*wr_thsz) + rem, SEEK_CUR); // Mov to current file position

            for(i = 0; i < write_amount; i++) {
                write(entry->file_fd, "", 1); // Write null bytes to expand the file.
            }

            void * (*worker)(void *);
            worker = (void *)(&client_handler);
            for(i = 0; i < nt - 1; i++) {
                thread_wr* d = malloc(sizeof(thread_wr));
                d -> sockfd = sock;
                d -> fd = new_fds[i];
                d -> size = wr_thsz;
                pthread_create(&(multiplex_threads[i]), NULL, worker, d);
            }

            if (errno != 0) {
                perror("Uh oh error after creating threads.");
                write_socket_err(sock, errno);
                return -1;
            }

            for(i = 0; i < nt; i++) {
                pthread_join(multiplex_threads[i], NULL);
                pthread_mutex_lock(&multiplex_lock);
                thread_taken[i + st] = 0;
                pthread_mutex_unlock(&multiplex_lock);
            }

            for(i = 0; i < nt; i++) {
                close(new_fds[i]);
            }
            free(new_fds);

        } else {
            write_socket_err(sock, EBADE);
        }


    } else {
        wrsz = write_socket_err(sock, EPERM);
    }
    return wrsz;
}

void handle_write(thread_wr* args) {
    size_t c = sizeof(struct sockaddr_in);
    struct sockaddr_in client;
    int conn;
    char* buf = malloc(sizeof(char)* (args->size + 20));
    conn = accept(args->sockfd, (struct sockaddr *)&client, (socklen_t*)&c);
    if (conn < 0) {
        perror("Failed on accept()");
    } else {
        printf("Conneción Accepted\n");
        ssize_t r = recv(args->sockfd, buf, args->size + 20, 0);

        if (r < 0) {
            //error on recv, errno is set
            write_socket_err(args->sockfd, errno);
            close(args->sockfd);
        } else {
            int netfd = convert_fd(retr_int( buf ));
            int ds = retr_int( buf + 4 );

            r = write(args->fd, buf + 8, ds);
            if (r < 0) {
                write_socket_err(args->sockfd, errno);
                close(args->sockfd);
            } else {
                char a[5];
                a[0] = '0';
                store_int(&( a[1] ), r);
                if (write(args->sockfd, a, 5) < 0) {
                    write_socket_err(args->sockfd, errno);
                    close(args->sockfd);
                }
            }
        }        

    }
    free(buf);
    free(args);

}

//Returns 0 if no error, otherwise value of ERR returned.
int check_flags(int sock, int flags) {

    if (flags == O_RDONLY || flags == O_WRONLY || flags == O_RDWR) {
        return 0;
    } else {
        // Bad flags
        return EINVAL;
    }
}

int write_socket_err(int sock, int err) {
    char a[5];
    a[0] = (char)'0';
    store_int(&( a[1] ), err);
    return write(sock, &a, 5);
}

int convert_fd(int fd) {
    return (fd * -1);
}
