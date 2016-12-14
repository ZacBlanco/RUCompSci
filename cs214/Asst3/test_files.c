#include "libnetfiles.h"

#define assert(x, y) lassert(x, y, __LINE__)

void lassert(int c1, char* msg, int line);

int failed = 0;
int total_assertions = 0;



void test_netopenclose();
void test_list_methods();
void test_netinit();

int main(int args, char ** argv) {

    test_netinit();
    test_netopenclose();
    test_list_methods();
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



void test_list_methods() {
    
    file_data* head = NULL;
    file_data* fd1 = new_node("test1", -1, -92, 0, O_RDONLY); // (file, socket, actual_fd, conntype, flags)
    add_filedata(&head, fd1);
    assert(head == fd1, "Filedata and head should be the same");
    assert(head != NULL, "Head should not be NULL");

    file_data* fd2 = search_filedata(&head, -92);
    assert(fd2 != NULL, "We should have found a node with a file descriptor of -92.");



}


void lassert(int c1, char* msg, int line) {
    if(!c1) {
        printf("============================================\n");
        printf("Assert condition failed %i != 1. on line %i of test_lols\n", c1, line);
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
