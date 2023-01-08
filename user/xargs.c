#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int
main(int argc, char* argv[]) {

    if (argc < 2) {
        fprintf(2, "xargs: wrong arguments");
        exit(1);
    }

    int pid, i;
    char* program;
    char* args[MAXARG];
    int args_index = 0;

    program = argv[1];
    for (i = 1; i < argc; i++) {
        args[args_index] = argv[i];
        args_index++;
    }

    pid = fork();

    if (pid < 0) {
        fprintf(2, "xargs: failed call fork");
        exit(1);
    }

    if (pid == 0) {
        int i, n;
        char line[1024];
        char* arg;
        int index = 0;

        while ((n = read(0, line, 1024)) > 0) {
            arg = (char*) malloc(sizeof line);

            for (i = 0; i < n; i++) {
                if (line[i] == ' ' || line[i] == '\n') { //end of the line
                    arg[index] = 0;
                    args[args_index] = arg;
                    args_index++;
                    index = 0;
                    arg = (char*) malloc(sizeof line);
                } else {
                    arg[index++] = line[i];
                }
            }

            arg[index] = 0;
            args[args_index] = 0;
        }
        exec(program, args);
    } else {
        wait(0);
    }

    exit(0);
}
