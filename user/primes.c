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
    while(read(left_p[0], &number, 4) != 0){ // the pipe have numbers
        printf("prime %d\n", number);
        int right_p[2];
        pipe(right_p);
        close(left_p[1]);
        int fork_id= fork();
        if(fork_id != 0){
            // in parent process
            int prime = number;
            // read from left and write to right
            // if(getpid() == 3){
            //     printf("enter\n");
            // }
            while(read(left_p[0], &number, 4) != 0){
                if(number % prime != 0){
                    write(right_p[1], &number, 4);
                }
            }
            close(left_p[0]);
            close(right_p[1]);
            close(right_p[0]);
            // if(getpid() == 3){
            //     printf("exit\n");
            // }
            break;
        }
        left_p[0] = right_p[0];
        left_p[1] = right_p[1];
        // if(getpid() == 14){
        //     printf("aaa");
        //     int r = read(left_p[0], &number, 4);
        //     printf("r: %d, number: %d", r, number);
        // }
    }
    wait(0);
    // printf("PID %d exit\n", getpid());
    exit(0);
}