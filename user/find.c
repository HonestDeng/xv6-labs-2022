#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char* path, const char* target){
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        close(fd);
        return;
    }
    
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        // printf("%s\n", de.name);
        if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0){
            continue;
        }
        if(de.inum==0){
            continue;
        }
        strcpy(buf, path);
        p = buf + strlen(buf); // p points to the end of buf
        *p++ = '/';
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        // printf("path: %s\n", buf);
        if(stat(buf, &st) < 0){
            printf("ls: cannot stat %s\b", buf);
            continue;
        }
        
        if(st.type==T_DEVICE || st.type == T_FILE){
            if(strcmp(de.name, target) == 0){
                printf("%s\n", buf);
            }
        }else{
            // if st is a directory
            find(buf, target);
        }
    }
    close(fd);
    return;
}

int main(int argc, char *argv[])
{
    if(argc != 3){
        fprintf(2,"3 arguments are required");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}