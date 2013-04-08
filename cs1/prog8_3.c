#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(int argc, const char *argv[]) {
    pid_t pid;

    if(argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int key = 10;

    pid = fork();

    if(pid != 0) {
        char buf[1];
        int msqid = msgget(key, IPC_CREAT|0666);
        printf("Filename: %s\n===================================\n", argv[1]);
        while(1) {
            msgrcv(msqid,(void *)buf, 1, 0, MSG_NOERROR);
            putchar(buf[0]);
        }
        close(pfd[0]);
        wait(NULL);
    } else {
        char buf[1];
        int msqid = msgget(key, IPC_CREAT|0666);
        FILE *f;
        close(pfd[0]);
        f = fopen(argv[1], "r");
        while(fscanf(f,"%c",buf) != EOF) {
            msgsnd(msqid,(void *)buf,1, IPC_NOWAIT);
        }
        fclose(f);
        close(pfd[1]);
    }
    return 0;
}