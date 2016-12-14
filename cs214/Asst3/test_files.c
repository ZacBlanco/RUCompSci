#include "libnetfiles.h"

#define assert(x, y) lassert(x, y, __LINE__)

void lassert(int c1, char* msg, int line);

int failed = 0;
int total_assertions = 0;



void test_netopenclose();
void test_list_methods();
void test_netinit();
void test_read();
void test_write();

int main(int args, char ** argv) {

    test_netinit();
    test_netopenclose();
    test_list_methods();
    test_read();
    test_write();

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


}

void test_read() {
    netserverinit("localhost");
    char buffer[BUFF_SIZE];
    int f1 = netopen("test.txt", O_RDONLY);
    ssize_t b = netread(f1, &buffer, 3);
    assert(b == 3, "Should have read 3 bytes");
    netclose(f1);
    int t = netread(-1, &buffer, O_RDONLY);
    assert(t == -1, "Should not have been able to read from descriptor of -1");
    perror("The error from bad fd");
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


int finish() {
    if (failed > 0) {
        printf("%i of %i assertion(s) failed to pass.\n", failed, total_assertions);
    } else {
        printf("%i assertion(s) passed successfully\n", total_assertions);
    }
    return failed > 0;
}
