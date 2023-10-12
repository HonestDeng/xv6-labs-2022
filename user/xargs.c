#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char* argv[]){
    char buf[512];
    char* p = buf;

    while(read(0, p, 1) == 1){  // read a character
        p++;
        if(*(p-1) == '\n'){
            *(p-1) = 0;
            char* sub_argv[MAXARG];
            sub_argv[0] = argv[1];
            sub_argv[1] = argv[2];
            sub_argv[2] = buf;
            if(fork() == 0){
                exec(argv[1], sub_argv);
            }
            wait(0);  // wait for child process to complete
            p = buf;
        }
    }
    exit(0);
}