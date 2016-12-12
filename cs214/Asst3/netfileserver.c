#include "netfileserver.h"

pthread_mutex_t mode_mutex;

filenode* head = NULL;

int main(int argc, char** argv) {



    printf("I am the fileserver\n");

    int sfd = socket(AF_INET, SOCK_STREAM, 0); // socket fd
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
    | suc=1 (1) | descriptor (4) |                            |
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
    +-----------+---------------------------------------------+
    | suc=1 (1) |                                             |
    +-----------+---------------------------------------------+
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
            printf("Got: %s", (char*)buff);
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
        

            printf("Init Operation received\n");

            int mode = (int)*(buffer + 1);
            if (mode == NFS_UN || mode == NFS_EX || mode == NFS_TR) {
                // Set the server type
                write(sock, "1\n", strlen("1\n"));
            } else {
                // Bad mode type
                unsigned char msg[19];
                msg[0] = (char)'0';
                union int_to_char t;
                t.a = INVALID_FILE_MODE;
                msg[1] = t.b[0];
                msg[2] = t.b[1];
                msg[3] = t.b[2];
                msg[4] = t.b[3];
                printf("filemode: %i\n", (int)(msg + 1));
                union int_to_char g;
                g.b[0] = msg[1];
                g.b[1] = msg[2];
                g.b[2] = msg[3];
                g.b[3] = msg[4];
                printf("filemode: %i\n", g.a);
                strcpy(&msg[5], "Bad mode type\n");
                write(sock, &msg, 19);
            }
            break;



        case '1':
            printf("Open Operation Received\n");
            return_code = -1;
            break;
        case '2':
            printf("Close Operation Received\n");
            return_code = -1;
            break;
        case '3':
            printf("Read Operation Received\n");
            return_code = -1;
            break;
        case '4':
            printf("Write Operation Received\n");
            return_code = -1;
            break;
        default:
            printf("Bad message\n");
            break;
    }

    return return_code;
}


void add_filenode(filenode* head, filenode* node) {
    if (head == NULL) {
        head = node;
    } else {
        filenode* curr = head;
        filenode* prev = head;
        while(curr != NULL) {
            prev = curr;
            curr = curr->next;
        }
        prev->next = node;
        node->next = NULL;

    }
}

// Returns 0 on success, -1 if wasn't found.
filenode* remove_filenode(filenode* head, int fd_selector) {
    filenode* curr = head;
    filenode* prev = head;
    while(curr != NULL) {
        if(curr->fd == fd_selector) {
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) {
        return NULL;
    } else {
        prev->next = curr->next;
    }

}

void free_filenode(filenode* node) {
    free(node->filename);
    node->next = NULL;
    free(node);
}