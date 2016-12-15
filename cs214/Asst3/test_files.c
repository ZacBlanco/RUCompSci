#include "libnetfiles.h"

#define assert(x, y) lassert(x, y, __LINE__)

void lassert(int c1, char* msg, int line);
int write_to_file(const char* str, char* filename);

int failed = 0;
int total_assertions = 0;

void test_netopenclose();
void test_list_methods();
void test_netinit();
void test_read();
void test_write();
void test_rw();
void test_extA();

int main(int args, char ** argv) {
    write_to_file("rutgers", "test.txt");
    write_to_file("rutgers", "things.txt");
    test_rw();
    test_netinit();
    test_netopenclose();
    test_list_methods();
    test_read();
    test_write();
    test_extA();
    return finish();

}

void test_extA() {
    
    write_to_file("", "perm1.test");
    write_to_file("", "perm2.test");
    write_to_file("", "perm3.test");
    write_to_file("", "perm4.test");
    write_to_file("", "perm5.test");
    write_to_file("", "perm6.test");
    write_to_file("", "perm7.test");
    write_to_file("", "perm8.test");
    write_to_file("", "perm8.test");

    netserverinit("localhost", NFS_UN);
    int fd1 = netopen("./perm1.test", O_RDWR);

    netserverinit("localhost", NFS_EX);
    assert(netopen("./perm1.test", O_WRONLY) == -1, "Should not be able to open Exclusive write when someone has rdwr");
    assert(netopen("./perm1.test", O_RDWR) == -1, "Should not be able to open Exclusive write when someone has rdwr");
    assert(netopen("./perm1.test", O_RDONLY) != -1, "Should be able to do exclusive on readonly");

    netserverinit("localhost", NFS_TR);
    assert(netopen("./perm1.test", O_WRONLY) == -1, "Should not be able to open trans write when someone has rdwr");
    assert(netopen("./perm1.test", O_RDWR) == -1, "Should not be able to open trans write when someone has rdwr");
    assert(netopen("./perm1.test", O_RDONLY) == -1, "Should not be able to do trans on readonly");

    netserverinit("localhost", NFS_UN);
    assert(netopen("./perm1.test", O_RDONLY) != -1, "Should be able to do un on readonly");
    assert(netopen("./perm1.test", O_WRONLY) != -1, "Should be able to do un on readonly");
    assert(netopen("./perm1.test", O_RDWR) != -1, "Should be able to do un on readonly");



    netserverinit("localhost", NFS_EX);
    fd1 = netopen("./perm2.test", O_RDWR);

    netserverinit("localhost", NFS_EX);
    assert(netopen("./perm2.test", O_WRONLY) == -1, "Should not be able to open Exclusive write when someone has rdwr");
    assert(netopen("./perm2.test", O_RDWR) == -1, "Should not be able to open Exclusive write when someone has rdwr");
    assert(netopen("./perm2.test", O_RDONLY) != -1, "Should be able to do exclusive on readonly");

    netserverinit("localhost", NFS_TR);
    assert(netopen("./perm2.test", O_WRONLY) == -1, "Should not be able to open trans write when someone has rdwr");
    assert(netopen("./perm2.test", O_RDWR) == -1, "Should not be able to open trans write when someone has rdwr");
    assert(netopen("./perm2.test", O_RDONLY) == -1, "Should not be able to do trans on readonly");

    netserverinit("localhost", NFS_UN);
    assert(netopen("./perm2.test", O_RDONLY) != -1, "Should be able to do un on readonly");
    assert(netopen("./perm2.test", O_WRONLY) == -1, "Should not be able to do un on writeonly");
    assert(netopen("./perm2.test", O_RDWR) == -1, "Should not be able to do un on readwr");


    netserverinit("localhost", NFS_TR);
    fd1 = netopen("./perm3.test", O_RDWR);

    netserverinit("localhost", NFS_EX);
    assert(netopen("./perm3.test", O_WRONLY) == -1, "Should not be able to open Exclusive write when someone has rdwr");
    assert(netopen("./perm3.test", O_RDWR) == -1, "Should not be able to open Exclusive write when someone has rdwr");
    assert(netopen("./perm3.test", O_RDONLY) == -1, "Should be able to do exclusive on readonly");

    netserverinit("localhost", NFS_TR);
    assert(netopen("./perm3.test", O_WRONLY) == -1, "Should not be able to open trans write when someone has rdwr");
    assert(netopen("./perm3.test", O_RDWR) == -1, "Should not be able to open trans write when someone has rdwr");
    assert(netopen("./perm3.test", O_RDONLY) == -1, "Should not be able to do trans on readonly");

    netserverinit("localhost", NFS_UN);
    assert(netopen("./perm3.test", O_RDONLY) == -1, "Should be able to do un on readonly");
    assert(netopen("./perm3.test", O_WRONLY) == -1, "Should not be able to do un on readonly");
    assert(netopen("./perm3.test", O_RDWR) == -1, "Should not be able to do un on readonly");




    netserverinit("localhost", NFS_UN);
    fd1 = netopen("./perm4.test", O_RDONLY);

    netserverinit("localhost", NFS_TR);
    assert(netopen("./perm4.test", O_WRONLY) == -1, "Should not be able to open trans write when someone has rdwr");
    assert(netopen("./perm4.test", O_RDWR) == -1, "Should not be able to open trans write when someone has rdwr");
    assert(netopen("./perm4.test", O_RDONLY) == -1, "Should not be able to do trans on readonly");

    netserverinit("localhost", NFS_UN);
    assert(netopen("./perm4.test", O_RDONLY) != -1, "Should be able to do un on readonly");
    assert(netopen("./perm4.test", O_WRONLY) != -1, "Should be able to do un on readonly");
    assert(netopen("./perm1.test", O_RDWR) != -1, "Should be able to do un on readonly");

    netserverinit("localhost", NFS_EX);
    assert(netopen("./perm4.test", O_WRONLY) == -1, "Should not be able to open Exclusive write when someone has rdwr");
    assert(netopen("./perm4.test", O_RDWR) == -1, "Should not be able to open Exclusive write when someone has rdwr");
    assert(netopen("./perm4.test", O_RDONLY) != -1, "Should be able to do exclusive on readonly");


    netserverinit("localhost", NFS_UN);
    fd1 = netopen("./perm5.test", O_RDONLY);
    netserverinit("localhost", NFS_UN);
    assert(netopen("./perm5.test", O_RDONLY) != -1, "Should be able to do un on readonly");
    assert(netopen("./perm5.test", O_WRONLY) != -1, "Should be able to do un on readonly");
    netserverinit("localhost", NFS_EX);
    assert(netopen("./perm5.test", O_WRONLY) == -1, "Should not be able to open Exclusive write when someone has rdwr");


}

void test_netinit() {

    int r = netserverinit(NULL, NFS_UN);
    assert(r == -1, "Should have gotten -1 on NULL arg to netserverinit");
    assert(errno != 0, "Errno should have been set if return is -1");
    
    r = netserverinit("______", NFS_UN);
    assert(r == -1, "Should have gotten -1 on \"_____\" arg to netserverinit");
    assert(errno != 0, "Errno should have been set if return is -1");

    r = netserverinit("localhost", NFS_UN);
    assert(r == 0, "Should be able to get simple connection to localhost.");

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
    netserverinit("localhost", NFS_UN);
    int fd1 = netopen("./test.txt", O_RDWR);
    int fd2 = netopen("./test.txt", O_WRONLY);
    int fd3 = netopen("./test.txt", O_RDONLY);
    
    assert(fd1 != -1, "Descriptor 1 should be good");
    assert(fd2 != -1, "Descriptor 2 should be good");
    assert(fd3 != -1, "Descriptor 3 should be good");
    assert(netwrite(fd1, "test1", 5) == 5, "Should be able to write 5 successful bytes");
    assert(netread(fd3, &buf, 5) == 5, "Should read 5 bytes");
    assert(netwrite(fd2, "test2", 5) == 5, "Should write 5 successful bytes with fd2");
    int nr = netread(fd1, buf, 2);
    printf("NR: %i", nr);
    assert( nr == 2, "read 2 successful bytes");

    netclose(fd2);
    netclose(fd3);
    netclose(fd1);
}

void test_write() {
    netserverinit("localhost", NFS_UN);
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
    netserverinit("localhost", NFS_UN);
    char buffer[BUFF_SIZE];
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
    fd1 = new_node("test2", -1, 7, 0, O_RDONLY);
    add_filedata(&head, fd1);
    fd1 = new_node("test3", -1, 14, 0, O_RDONLY);
    add_filedata(&head, fd1);

    
    fd2 = search_filedata(&head, -92);
    assert(fd2 != NULL, "We should have found a node with a file descriptor of -92.");
    fd2 = search_filedata(&head, 7);
    assert(fd2 != NULL, "We should have found a node with a file descriptor of 7.");
    fd2 = search_filedata(&head, 14);
    assert(fd2 != NULL, "We should have found a node with a file descriptor of 14.");

    file_data* fd4 = search_filedata_byname(&head, "test2");
    assert(fd4 != NULL, "Should have found node test2");
    fd4 = search_filedata_byname(&head, "test1");
    assert(fd4 != NULL, "Should have found node test1");
    fd4 = search_filedata_byname(&head, "test3");
    assert(fd4 != NULL, "Should have found node test3");

    fd4 = search_filedata_byname(&head, "abc123");
    assert(fd4 == NULL, "Shouldn't have been able to find abc123");


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
