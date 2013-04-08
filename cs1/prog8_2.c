#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <pthread.h>

#define SHMSZ 1024

int shmid;
char *data, *od;

void *reciever() {
    while(1) {
        if(strcmp(od, data) != 0) {
            printf("%s\nMsg: ",data);
            strcpy(od, data);
        }
    }
}

void *sender() {
    while(1) {
        printf("Msg: ");
        scanf("%s", data);
        strcpy(od, data);
    }
}

int main(int argc, const char *argv[]) {
    int rR, rS;
    pthread_t threadR, threadS;

    if(argc != 2) {
        fprintf(stderr, "Usage: %s <key>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int key = 10;
    //sscanf(argv[2], "%d", &key);

    od = (char *) malloc(SHMSZ);
    printf("Hello\n");
    shmid = shmget((key_t) key, SHMSZ, IPC_CREAT|0666);
    if(shmid < 0) {
        fprintf(stderr, "Share segment creation failed!\n");
        exit(EXIT_FAILURE);
    }
    printf("Memory Mount Complete.\n");
    data = (char *) shmat(shmid, NULL, 0);
    strcpy(od, data);
    printf("Server started with key: %d\n=================================\n\n", key);

    if ((rR=pthread_create(&threadR, NULL, reciever, NULL))) {
        printf("ThreadR creation failed: %d\n", rR);
    }
    if ((rS=pthread_create(&threadS, NULL, sender, NULL))) {
        printf("ThreadS creation failed: %d\n", rS);
    }
    pthread_join(threadR, NULL);
    pthread_join(threadS, NULL);
    return 0;
}
