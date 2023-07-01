#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    char ln = '\n';
    if(argc != 2){
        char* msg = "One argument required";
        write(1, msg, strlen(msg));
        write(1, &ln, 1);
        exit(1);
    }
    int t = atoi(argv[1]);
    sleep(t);
    exit(0);
}