#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    char ln = '\n';
    if(argc > 1){
        char* err = "No argument required";
        write(2, err, strlen(err));
        write(2, &ln, 1);
        exit(1);
    }
    int p[2];
    pipe(p);
    if(fork() == 0){
        // child process
        char byt;
        read(p[0], &byt, 1);  // read from pipe
        char pid_c = '0' + getpid();
        write(1, &pid_c, 1);
        char* msg = ": received ping";
        write(1, msg, strlen(msg));
        write(1, &ln, 1);
        close(p[0]);  // close read end

        // write to pipe
        byt = 'b';
        write(p[1], &byt, 1);
        close(p[1]);  // close write end
    }else{
        char byt = 'a';
        write(p[1], &byt, 1);  // write to pipe
        // wait(0);  // wait for child finishing
        close(p[1]);  // close write end

        // read from pipe
        read(p[0], &byt, 1);
        char pid_c = '0' + getpid();
        write(1, &pid_c, 1);
        char* msg = ": received pong\n";
        write(1, msg, strlen(msg));
        close(p[0]);  // close read end
    }
    exit(0);
}