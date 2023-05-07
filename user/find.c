#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

int find(char *path, char *filename);

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("find: invalid arguments\n");
    }

    int status = find(argv[1], argv[2]);
    exit(status);
}

int find(char *path, char *filename) {
    char buf[512];
    char *name, *p;
    int fd;
    struct stat st;
    struct dirent de;

    if((fd = open(path, O_RDONLY)) < 0) {
        printf("find: cannot open %s\n", path);
        return -1;
    }

    if(fstat(fd, &st) < 0) {
        printf("find: cannot stat %s\n", path);
        close(fd);
        return -1;
    }

    switch (st.type) {
        case T_DEVICE:
        case T_FILE:
            name = path;
            // get position of filename
            for(int i = strlen(path) - 1; i >= 0; i--) {
                if(path[i] == '/') {
                    name = &path[i+1];
                    break;
                }
            }
            if(!strcmp(name, filename)) {
                printf("%s\n", path);
            }
            break;

        // if path is directory
        case T_DIR:
            if(strlen(path)+1+DIRSIZ+1 > sizeof(buf)) {
                printf("find: path too long\n");
                close(fd);
                return -1;
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)) {
                // excpet for "." and ".."
                if(de.inum == 0 || !strcmp(de.name, ".") || !strcmp(de.name, "..")) {
                    continue;
                }
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = '\0';
                find(buf, filename);
            }
            break;
    }
    
    close(fd);
    return 0;
}
