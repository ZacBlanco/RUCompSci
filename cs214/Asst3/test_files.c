#include "libnetfiles.h"

void test_netopenclose();

int main(int args, char ** argv) {
    
    test_netopenclose();
    return 0;

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
