#include "kernel/types.h"
#include "user/user.h"



int main() {
    int ping[2];
    int pong[2];
    char buf[] = {'x'};
    int length = sizeof(buf);
    pipe(ping);
    pipe(pong);
    if (fork() == 0) {
        close(ping[1]);
        close(pong[0]);
        if(read(ping[0],buf,sizeof(buf)) != length) {
            printf("1->2 r error");
            exit();
        }
        printf("%d: received ping\n",getpid());
        if(write(pong[1],buf,length) != length){
            printf("2->1 w error");
            exit();
        }
        exit();
    }
    close(ping[0]);
    close(pong[1]);
        if(write(ping[1],buf,sizeof(buf)) != length){
        exit();
    }
        if(read(pong[0],buf,sizeof(buf)) != length) {
        
        exit();
    }

    printf("%d: received pong\n",getpid());
    wait();
    exit();
}