#include "libnetfiles.h"

#define assert(x, y) lassert(x, y, __LINE__)

void lassert(int c1, char* msg, int line);
int write_to_file(const char* str, char* filename);

int failed = 0;
int total_assertions = 0;

void test_netopenclose();
void test_list_methods();
void test_min_max();
void test_multiplex();
void test_netinit();
void test_read();
void test_write();
void test_rw();

int main(int args, char ** argv) {
    //write_to_file("rutgers", "test.txt");
    //test_netinit();
   // test_netopenclose();
  //  test_list_methods();
 //   test_multiplex();
//    test_min_max();
    test_read();
    //test_write();
    //test_rw();
    return finish();

}

void test_netinit() {

    int r = netserverinit(NULL);
    assert(r == -1, "Should have gotten -1 on NULL arg to netserverinit");
    assert(errno != 0, "Errno should have been set if return is -1");
    
    r = netserverinit("______");
    assert(r == -1, "Should have gotten -1 on \"_____\" arg to netserverinit");
    assert(errno != 0, "Errno should have been set if return is -1");

    r = netserverinit("localhost");
    assert(r == 0, "Should be able to get simple connection to localhost.");

}

void test_multiplex() {
    int * thread_taken = calloc(10, sizeof(int));
    int nt;
    int st;
    int rd_sz = 5000;

    //Test Zero'd array
    nt = get_max_multiplex(rd_sz, 0, thread_taken);
    st = get_max_multiplex(rd_sz, 1, thread_taken);
    assert(nt == 3, "Should return 3 threads available.");
    //printf("Number of threads returned: %i\n", nt);
    assert(st == 0, "Should return start point from 0.");

    //Test fragmented array
    int i;
    for(i = 0; i < 10; i += 3) {
        thread_taken[i] = 1;
    }
    nt = get_max_multiplex(rd_sz, 0, thread_taken);
    st = get_max_multiplex(rd_sz, 1, thread_taken);
    assert(nt == 2, "Should return 2 threads available.");
    //printf("Number of threads returned: %i\n", nt);
    assert(st == 1, "Should return 1st index.");
    printf("Starting location: %i\n", st);

    //Test Half available
    for (i = 0; i < 9; i++) {
        thread_taken[i] = 0;
    }
    thread_taken[5] = 1;
    rd_sz = 10000;
    nt = get_max_multiplex(rd_sz, 0, thread_taken);
    st = get_max_multiplex(rd_sz, 1, thread_taken);
    assert(nt == 5, "Should return 5 threads available.");
    //printf("Number of threads returned: %i\n", nt);
    assert(st == 0, "Should return 0th index.");
    //printf("Starting location: %i\n", st);
    
    
}

void test_netopenclose() {
    int fd = netopen("./test.txt", O_RDONLY);
    if (fd < 0) {
        printf("Open failed.\n");
    }

    printf("fd received from netopen: %i\n", fd);

    int result = netclose(fd);
    if (result) {
        printf("Close success.\n");
    } else {
        printf("Close failure.\n");
    }

}

void test_rw() {
    char buf[BUFF_SIZE];
    netserverinit("localhost");
    int fd1 = netopen("./test.txt", O_RDWR);
    int fd2 = netopen("./test.txt", O_WRONLY);
    int fd3 = netopen("./test.txt", O_RDONLY);
    
    assert(fd1 != -1, "Descriptor 1 should be good");
    assert(fd2 != -1, "Descriptor 2 should be good");
    assert(fd3 != -1, "Descriptor 3 should be good");
    assert(netwrite(fd1, "test1", 5) == 5, "Should be able to write 5 successful bytes");
    assert(netread(fd3, &buf, 5) == 5, "Should read 5 bytes");
    assert(netwrite(fd2, "test2", 5) == 5, "Should write 5 successful bytes with fd2");
    assert(netread(fd1, buf, 2) == 2, "read 2 successful bytes");
    
    netclose(fd2);
    netclose(fd3);
    netclose(fd1);
}

void test_write() {
    netserverinit("localhost");
    printf("Flag sent with open: %i\n", O_RDWR);
    int fd = netopen("./things.txt", O_RDWR);
    char* msg = "overwriting rutgers.";
    int len = strlen(msg);
    int bts = netwrite(fd, msg, len);
    assert(bts == len, "number of bytes written should be the same.");
    assert(errno == 0, "errno shouldn't have been set");
    netclose(fd);

    // Test on readonly 
    write_to_file("", "test.txt");
    fd = netopen("./things.txt", O_RDONLY);
    bts = netwrite(fd, msg, strlen(msg));
    assert(bts == -1, "Should not have been able to write");
    assert(errno != 0, "Errno should have been set");
    netclose(fd);

    fd = netopen("./things.txt", O_RDWR);
    bts = netwrite(fd, msg, strlen(msg));
    assert(bts == len, "number of bytes written should be the same.");
    assert(errno == 0, "errno shouldn't have been set");
    netclose(fd);


    fd = netopen("./things.txt", O_RDWR);
    msg = "EL OH EL";
    len = strlen(msg);
    bts = netwrite(fd, msg, strlen(msg));
    assert(bts == len, "number of bytes written should be the same.");
    assert(errno == 0, "errno shouldn't have been set");
    
    msg = "test test test\n";
    len = strlen(msg);
    bts = netwrite(fd, msg, strlen(msg));
    assert(bts == len, "number of bytes written should be the same. 2nd write in a row");
    assert(errno == 0, "errno shouldn't have been set on 2nd write");
    netclose(fd);

}

void test_read() {
    netserverinit("localhost");
    /*char buffer[BUFF_SIZE];
    int f1 = netopen("test.txt", O_RDONLY);
    ssize_t b = netread(f1, &buffer, 3);
    assert(b == 3, "Should have read 3 bytes");
    netclose(f1);
    int t = netread(-1, &buffer, O_RDONLY);
    assert(t == -1, "Should not have been able to read from descriptor of -1");
    perror("The error from bad fd");


    //Test multiple reads
    f1 = netopen("test.txt", O_RDONLY);
    b = netread(f1, &buffer, 3);
    assert(b == 3, "Should have read 3 bytes");
    netclose(f1);
    
    f1 = netopen("test.txt", O_RDONLY);
    b = netread(f1, &buffer, 4);
    assert(b == 4, "Should have read 3 bytes");
    netclose(f1);
    
    f1 = netopen("test.txt", O_RDONLY);
    b = netread(f1, &buffer, 5);
    assert(b == 5, "Should have read 3 bytes");
    netclose(f1);

    f1 = netopen("test.txt", O_RDONLY);
    b = netread(f1, &buffer, 20);
    assert(b == 7, "Should have read 7 bytes on attempted 20 byte read");
    netclose(f1);

    f1 = netopen("test.txt", O_RDONLY);
    b = netread(f1, &buffer, 4);
    assert(b == 4, "Should have read 4 bytes first read before close");
    b = netread(f1, &( buffer[3] ), 3);
    assert(b == 3, "Should have read 7 bytes on attempted 3 byte read");
    netclose(f1);

    f1 = netopen("test.txt", O_RDONLY);
    b = netread(f1, &buffer, 5);
    assert(b == 5, "Should have read 5 bytes first read before close");
    b = netread(f1, &( buffer[4] ), 3);
    assert(b == 2, "Should have read last 2 bytes on attempted 3 byte read");
    netclose(f1);


    //Test different modes.
    
    f1 = netopen("test.txt", O_WRONLY);
    b = netread(f1, &buffer, 1);
    printf("bytes: %zd\n", b);
    printf("O_RDONLY: %i, O_WRONLY: %i, O_RDWR: %i\n", O_RDONLY, O_WRONLY, O_RDWR);
    assert(b == -1, "Should not have been able to read on writeonly file");
    netclose(f1);

    f1 = netopen("test.txt", O_RDWR);
    b = netread(f1, &buffer, 6);
    assert(b == 6, "Should have been able to write on file.");
    netclose(f1);
*/
    //Test threaded version
    
    char bigbuff[4000];
    int f2 = netopen("big.txt", O_RDONLY);
    ssize_t b = netread(f2, &bigbuff, 3500);
    errno = retr_int(bigbuff + 1);
    perror("Error from bigread");
    assert(b == 3500, "Threads should work.");
    printf("Bytes read: %i\n", b);
    netclose(f2);

}

void test_min_max() {
    
    assert(min(0, 10) == 0, "0 should be less than 10");
    assert(min(10, 0) == 0, "0 should be less than 10");
    assert(max(10, 0) == 10, "10 should be greater than 0");
    assert(max(0, 10) == 10, "10 should be greater than 0");
    
}

void test_list_methods() {
    
    file_data* head = NULL;
    file_data* fd1 = new_node("test1", -1, -92, 0, O_RDONLY); // (file, socket, actual_fd, conntype, flags)
    add_filedata(&head, fd1);
    assert(head == fd1, "Filedata and head should be the same");
    assert(head != NULL, "Head should not be NULL");

    file_data* fd2 = search_filedata(&head, -92);
    assert(fd2 != NULL, "We should have found a node with a file descriptor of -92.");
    fd2 = search_filedata(&head, 0);
    assert(fd2 == NULL, "Should get NULL on item that doesn't exist");
    file_data* fd3 = remove_filedata(&head, -92);
    assert(head == NULL, "Head should be null after removing only item.");
    assert(fd3 == fd1, "Removed node should be the same as original node.");

    fd2 = search_filedata(&head, 0);
    assert(fd2 == NULL, "Should get Null searching on null list");

    add_filedata(&head, fd1);
    fd1 = new_node("test1", -1, 7, 0, O_RDONLY);
    add_filedata(&head, fd1);
    fd1 = new_node("test1", -1, 14, 0, O_RDONLY);
    add_filedata(&head, fd1);

    
    fd2 = search_filedata(&head, -92);
    assert(fd2 != NULL, "We should have found a node with a file descriptor of -92.");
    fd2 = search_filedata(&head, 7);
    assert(fd2 != NULL, "We should have found a node with a file descriptor of 7.");
    fd2 = search_filedata(&head, 14);
    assert(fd2 != NULL, "We should have found a node with a file descriptor of 14.");

}


void lassert(int c1, char* msg, int line) {
    if(!c1) {
        printf("============================================\n");
        printf("Assert condition failed %i != 1. on line %i of test file\n", c1, line);
        printf("%s\n", msg);
        printf("============================================\n");
        failed++;
    }
    total_assertions++;
    
}

int write_to_file(const char* str, char* filename) {
    int success = 1; //Assume it to first always be successfull
    FILE* f = fopen(filename, "w");
    if (f) { // No error when attempting to open file.
        if (fputs(str, f) == EOF) { //EOF when error
            // An Error occurred when writing to file
            fprintf(stderr, "Error when writing to file %s", filename);
            success = 0;
        }
        fclose(f);
        // printf("Wrote to file\n");
    } else { success = 0;
        fprintf(stderr, "Could not open a pointer to the file %s\n", filename);
    }
    
    return success;
}



int finish() {
    if (failed > 0) {
        printf("%i of %i assertion(s) failed to pass.\n", failed, total_assertions);
    } else {
        printf("%i assertion(s) passed successfully\n", total_assertions);
    }
    return failed > 0;
}
