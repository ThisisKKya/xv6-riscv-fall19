#include "kernel/types.h"
#include "user/user.h"

int main(int argn, char* argv[2]) {
    if (argn != 2) {
        fprintf(2,"2 argv expected\n");
        exit();
    }
    int sleepTime = atoi(argv[1]);
    printf("sleep a little\n");
    sleep(sleepTime);
    exit();
}