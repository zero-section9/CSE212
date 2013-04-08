#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char *argv[]) {
  int pfd[2];
  pid_t pid;

  if(argc != 2) {
    fprintf(stderr, "Usage: %s <string>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if(pipe(pfd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  pid = fork();

  if(pid != 0) {
    char buf;
    close(pfd[1]);
    printf("Filename: %s\n===================================\n", argv[1]);
    while(read(pfd[0], &buf, 1) > 0) {
      putchar(buf);
    }
    close(pfd[0]);
    wait(NULL);
  } else {
    char buf;
    FILE *f;
    close(pfd[0]);
    f = fopen(argv[1], "r");
    while(fscanf(f,"%c",&buf) != EOF) {
      write(pfd[1],&buf,1);
    }
    fclose(f);
    close(pfd[1]);
  }
  return 0;
}
