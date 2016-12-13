#include "netfileserver.h"

pthread_mutex_t mode_mutex;

file_data* head = NULL;

int main(int argc, char** argv) {



    printf("I am the fileserver\n");

    int sfd = socket(AF_INET, SOCK_STREAM, 0); // socket fd
    // set SO_REUSEADDR on a socket to true (1):
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
            printf("Connecion Accepted\n");

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

     - On a close operation we have a first bit of 2. After
    which we care about 1 argument - The file descriptor.

    Expected Message
    +---------+----------------+-----------+------------------+
    | op. (1) | descriptor (4) | size (4)  |                  |
    +---------+----------------+-----------+------------------+

    - Responses differ in type based on failure/success

    Expected Response on Success
    +-----------+---------------------+-----------------------+
    | suc=1 (1) | size (4) | data (n) |                       |
    +-----------+---------------------+-----------------------+
    Expected Response on Failure
    +-----------+-----------+----------------+----------------+
    | suc=0 (1) | errno (4) | errm messg (n) |                |
    +-----------+-----------+----------------+----------------+


    ------ '4' - Write Operation ------ 

    - On a close operation we have a first bit of 2. After
    which we care about 1 argument - The file descriptor.

    Expected Message
    +---------+----------------+----------+---------+---------+
    | op. (1) | descriptor (4) | size (4) | data(n) |         |
    +---------+----------------+----------+---------+---------+

    - Responses differ in type based on failure/success

    Expected Response on Success
    +-----------+---------+-----------------------------------+
    | suc=1 (1) | size(4) |                                   |
    +-----------+---------+-----------------------------------+
    Expected Response on Failure
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

    return return_code;
}


// Returns the number of bytes written back to the client.
int close_op(int sock, const char* buffer, ssize_t sz) {
    int badf = 0; // true if we need to send an error; 
    int sz_wr = 0;
    printf("Close Operation Received:\n");
    if (sz < 5) {
        // Message did not contain enough bytes to process
        badf = 1;
    } else {
        int fd = retr_int( buffer + 1 );
        file_data* closen = search_filedata(head, fd);
        if (closen == NULL) {
            //File did not exist.
            badf = 1;
        } else {
            remove_filedata(head, fd);
            free_filedata(closen);
            close(fd);
        }
    }

    // Write depending on whether file descriptor was bad.
    if (badf) {
        char* mg = "Bad file descriptor";
        int len = 5 + strlen(mg);
        char msg[len];
        msg[0] = '0';
        store_int(&(msg[1]), EBADF);
        strcpy(&(msg[5]), mg);
        sz_wr = write(sock, msg, len);
    } else {
        char msg[1] = { '1' };
        sz_wr = write(sock, &msg, 1);
    }

    return sz_wr;

}


int init_op(int sock, const char* buffer, ssize_t sz) {

    printf("Init Operation received\n");

    int mode = (int)*(buffer + 1);
    
    if (check_filemode(sock, mode)){}

}

//Return number of bytes written to client.
int open_op(int sock, const char* buffer, ssize_t sz) {

    printf("Open Operation Received\n");
    int wrsz = 0; //write size;
    if (sz < 9) {
        wrsz = write_socket_err(sock, EINVAL);
    } else {
        int fmode = retr_int(buffer + 1);
        int flags = retr_int(buffer + 5);
        char* filepath = malloc(sizeof(char) * (sz - 9)); //sz-9 is the number of chars for filename
        filepath[0] = '\0';
        strncpy(filepath, buffer + 9, sz-9);
        // printf("buffer: %s fmode: %i flags: %i filepath: %s\n", buffer, fmode, flags, filepath);

        int fd = 0;
        int err = 0;
        // err = check_filemode(sock, fmode);
        if ( !err ) {
            err = check_flags(sock, flags);
        }

        // If there was no error on filemode or flags then we're ok.
        if (!err) {
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

        if (!fd) { // open failed - get error
            printf("Bad File. Errno - %s\n", strerror(errno));
            wrsz = write_socket_err(sock, errno);
        } else {

            char a[5];
            a[0] = '1';
            store_int(&( a[1] ), fd);
            wrsz = send(sock, &a, 5, 0);
            // printf("fd: %i\n", fd);
        }
    }
    return wrsz;
}

int read_op(int sock, const char* buffer, ssize_t sz){
    printf("Read Operation Received\n");
    int wrsz = 0;
    file_data* entry = NULL;
    if (sz < 9) {
        wrsz = write_socket_err(sock, EINVAL);    
    } else {

        int ffd = retr_int(buffer + 1);
        entry = search_filedata(head, ffd);
        
    }

    if (entry == NULL) {
        wrsz = write_socket_err(sock, EINVAL);
    } else if ( entry->flags == O_RDONLY || entry->flags == O_RDWR ){

        int rd_sz = retr_int(buffer + 5);
        int datasz = rd_sz + 5; // 1 for success and 4 for bytes read.
        // Entry not null -  read and return data        
        char* data = malloc(sizeof(char)*(datasz));
        ssize_t bts_read = read(entry->file_fd, data + 5, datasz);

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
    } else { //File descriptor was write only
        wrsz = write_socket_err(sock, EPERM);
    }
    return wrsz;

}

int write_op(int sock, const char* buffer, ssize_t sz) {
    printf("Write Operation Received\n");
    int wrsz = 0;
    file_data* entry = NULL;
    if (sz < 9) {
        wrsz = write_socket_err(sock, EINVAL);    
    } else {

        int ffd = retr_int(buffer + 1);
        entry = search_filedata(head, ffd);
        
    }
    printf("Entry is: %p.\n", entry);

    if (entry == NULL) {
        wrsz = write_socket_err(sock, EINVAL);
    } else if(entry->flags == O_RDWR || entry->flags == O_WRONLY){

        int write_amount = retr_int(buffer + 5);
        ssize_t bts_written = write(entry->file_fd, buffer + 9, write_amount);

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
    }
    return wrsz;
}


void add_filedata(file_data* head, file_data* node) {
    if (head == NULL) {
        head = node;
    } else {
        file_data* curr = head;
        file_data* prev = head;
        while(curr != NULL) {
            prev = curr;
            curr = curr->next;
        }
        prev->next = node;
        node->next = NULL;

    }
}

file_data* search_filedata(file_data* head, int fd_selector) {
    file_data* curr = head;
    while( curr != NULL ) {
        if (curr->file_fd == fd_selector) {
            return curr;
        }
    }
    return NULL;
}

// Returns removed node on success, NULL if not found.
file_data* remove_filedata(file_data* head, int fd_selector) {
    file_data* curr = head;
    file_data* prev = head;

    if (curr->file_fd == fd_selector) {
        head = head->next;
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

void free_filedata(file_data* node) {
    free(node->filename);
    node->next = NULL;
    free(node);
}


//Returns 0 if no error. Otherwise value of ERR returned
int check_filemode(int sock, int mode) {
    
    if (mode == NFS_UN || mode == NFS_EX || mode == NFS_TR) {
        // Set the server type
        return 0;
    } else {
        // Bad mode type
        // unsigned char msg[19];
        // msg[0] = (char)'0';
        // store_int(msg + 1, INVALID_FILE_MODE);
        // int num = retr_int(msg + 1);
        // // printf("filemode: %i\n", num);
        // strcpy(&(msg[5]), "Bad mode type\n");
        return EINVAL;
    }
}

//Returns 0 if no error, otherwise value of ERR returned.
int check_flags(int sock, int flags) {

    if (flags == O_RDONLY || flags == O_WRONLY || flags == O_RDWR) {
        return 0;
    } else {
        // Bad flags
        // unsigned char msg[19];
        // msg[0] = (char)'0';
        // store_int(msg + 1, INVALID_FLAG);
        // int num = retr_int(msg + 1);
        // strcpy(&(msg[5]), "Bad mode type\n");
        // write(sock, &msg, 19);
        return EINVAL;
    }
}

int write_socket_err(int sock, int err) {
    char a[5];
    a[0] = (char)'0';
    store_int(&( a[1] ), err);
    write(sock, &a, 5);
}
