#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READ 0
#define WRITE 1

__attribute__((noreturn))
void primes(int input_buf[2]) 
{
 
  int i, pid;
  int output_buf[2];

  read(input_buf[READ], &i, sizeof(i));

  if (i == -1) {
    exit(0);
  }
  printf("primes: %d\n", i);

  pipe(output_buf);

  pid = fork();

  if (pid < 0) {
    printf("Create process failed");
    exit(1);
  } else if (pid == 0) {
    close(output_buf[WRITE]);
    close(input_buf[READ]);

    primes(output_buf);
  } else {
    close(output_buf[READ]);

    int num;
    while (read(input_buf[READ], &num, sizeof(num)) && num != -1) {
      if (num % i != 0) {
        write(output_buf[WRITE], &num, sizeof(num));
      }
    }

    num = -1; //done flag
    write(output_buf[WRITE], &num, sizeof(num));

    wait(0);
    exit(0);
  }
}

int main()
{
  int i, pid;
  int buf[2]; 

  pipe(buf);

  pid = fork();

  if (pid < 0) {
    printf("Create process failed");
    exit(1);
  } else if (pid == 0) {
    close(buf[WRITE]);
    
    primes(buf);

    exit(0);
  } else {
    close(buf[READ]);

    for (i = 2; i < 36; i++) {
      write(buf[WRITE], &i, sizeof(i));
    }

    i = -1; //done flag
    write(buf[WRITE], &i, sizeof(i));
  }

  wait(0);

  exit(0);
}
