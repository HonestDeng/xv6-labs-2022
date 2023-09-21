#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    int N = 35;
    int left_p[2];
    pipe(left_p);
    for(int i=2; i<=N;i++){
        // write number to pipe
        write(left_p[1], &i, 4);
    }
    int number;
    while(read(left_p[0], &number, 4) != 0){ // if the pipe have numbers
        printf("prime %d\n", number);
        int right_p[2];
        pipe(right_p);
        int fork_id= fork();
        if(fork_id != 0){
            // in parent process
            close(left_p[1]);
            int prime = number;
            // read from left and write to right
            while(read(left_p[0], &number, 4) != 0){
                if(number % prime != 0){
                    write(right_p[1], &number, 4);
                }
            }
            close(left_p[0]);
            close(right_p[1]);
            close(right_p[0]);
            break;
        }
        // in child process
        close(left_p[1]);
        left_p[0] = right_p[0];
        left_p[1] = right_p[1];
    }
    wait(0);
    exit(0);
}