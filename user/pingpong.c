#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    if(argc > 1){
        printf("No argument required\n");
        exit(1);
    }
    int p[2];
    pipe(p);
    if(fork() == 0){
        // child process
        char byt;
        read(p[0], &byt, 1);  // read from pipe
        int pid_c = getpid();
        char* msg = "received ping";
        printf("%d: %s\n", pid_c, msg);
        close(p[0]);  // close read end of pipe

        // write to pipe
        byt = 'b';
        write(p[1], &byt, 1);
        close(p[1]);  // close write end
    }else{
        // father process
        char byt = 'a';
        write(p[1], &byt, 1);  // write to pipe
        close(p[1]);  // close write end

        // read from pipe
        read(p[0], &byt, 1);
        int pid_c = getpid();
        printf("%d: received pong\n", pid_c);
        close(p[0]);  // close read end
    }
    exit(0);
}