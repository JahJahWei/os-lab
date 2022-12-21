#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READ 0
#define WRITE 1

int
main()
{
    char buf[4];
    int parent_buf[2];
    int child_buf[2];

    int pid;

    pipe(parent_buf);
    pipe(child_buf);

    pid = fork();

    if (pid < 0) {
        printf("Faild create process");
        exit(1);
    }

    //read: 0, write: 1
    if (pid == 0) {
        close(child_buf[READ]);
        close(parent_buf[WRITE]);

        read(parent_buf[READ], &buf, 4);
        printf("%d: received %s\n", pid, buf);

        write(child_buf[WRITE], "pong", 4);
        close(child_buf[WRITE]);
    } else {
        close(parent_buf[READ]);
        close(child_buf[WRITE]);

        write(parent_buf[WRITE], "ping", 4);
        close(parent_buf[WRITE]);

        read(child_buf[READ], &buf, 4);
        printf("%d: received %s\n", pid, buf);
    }

    exit(0);
}