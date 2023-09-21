#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if(argc != 2){
        // check correct argument
        printf("One argument required\n");
        exit(1);
    }
    // convert string to int
    int t = atoi(argv[1]);
    sleep(t);  // syscall
    exit(0);
}