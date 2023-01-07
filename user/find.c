#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char * path)
{
    static char buf[DIRSIZ+1];
    char *p;

    for (p = path + strlen(path); p >= path && *p != '/'; p--);
    p ++;
    if (strlen(p) >= DIRSIZ) return p;
    memmove(buf, p, strlen(p));
    buf[strlen(p)] = 0;
    return buf;
}

char*
add_slash(char* buf, char* path)
{
    char* p;

    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';

    return p;
}

void 
find(char *path, char *file_name)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (st.type == T_FILE) {
        if (!strcmp(fmtname(path), file_name)) {
            printf("%s\n", path);
            return;
        }
    } else if (st.type == T_DIR) {
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
            printf("find: path too long\n");
            return;
        }

        p = add_slash(buf, path);

        while(read(fd, &de, sizeof(de))){
            if(de.inum == 0)
                continue;

            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;

            if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
                continue;

            find(buf, file_name);
        }
    }

    close(fd);
}

int 
main(int argc, char *argv[]) 
{
    if (argc < 3) {
        fprintf(2, "find: wrong arguments\n");
        exit(1);
    }

    find(argv[1], argv[2]);

    exit(0);
}
