#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUF_SIZE  100

void printps(char *msg, pid_t pid1, pid_t pid2, pid_t pid3) {
    char cmd[100];
    sprintf(cmd, "echo %s; ps -l", msg);
    system(cmd);
}

void son(pid_t gpid, pid_t fpid) {
    char msg[70];
    sprintf(msg, "\"Hello! I'm %d, Father: %d, GrandFather: %d\n\"", getpid(), gpid, fpid);
    printps(msg, getpid(), gpid, fpid);
    system("nohup ls");
    printf("I'm going to sleep.");
    sleep(10);
    printf("I'm awake.");
}

void father(pid_t gpid) {
    pid_t fpid = getpid();
    pid_t pid = fork();
    if(pid > 0) {
        char msg[70];
        sprintf(msg, "\"Hello! I'm %d, Father: %d, Son: %d\n\"", getpid(), gpid, pid);
        printps(msg, getpid(), gpid, pid);
    } else if(pid == 0) {
        son(gpid, fpid);
    } else {
        fprintf(stderr, "Error creating son: %d\n", pid);

    }
}

void grandfather(pid_t fpid) {
    char msg[70];
    sprintf(msg, "\"Hello! I'm %d with a son %d\n\"",getpid(), fpid);
    printps(msg, getpid(), fpid, 0);
    sleep(15);
    printps(msg, getpid(), fpid, 0);
    waitpid(fpid, NULL, 0);
}

int main(void) {
    pid_t pid;
    pid_t gpid = getpid();
    pid = fork();
    if(pid > 0) {
        grandfather(pid);
    } else if(pid == 0){
        father(gpid);
    } else {
        fprintf(stderr, "Error creating father: %d \n", pid);

    }
    return 0;
}
