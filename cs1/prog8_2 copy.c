#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/ipc.h>

#define SHMSZ 1024

void server(key_t key) {
    int shmid;
    void *segment;
    int *connections, oc, nc;
    int *running;
    char *data, *od;
    od = (char *) malloc(768);
    printf("Starting Server... \n");
    shmid = shmget(key, SHMSZ, IPC_CREAT|0666);
    if(shmid < 0) {
        fprintf(stderr, "Share segment creation failed!\n");
        exit(EXIT_FAILURE);
    }
    printf("Memory Mount Complete.\n");
    segment = shmat(shmid, NULL, 0);
    running = (int *) segment;
    *running = 1;
    printf("Server started with key: %d\n=================================\n\n", key);
    connections = (int *) segment + sizeof(int);
    oc = *connections;
    printf("Connections: %d\n", oc);
    data = (char *) segment + (2 * sizeof(int));
    strcpy(od, data);
    while(1) {
        nc = *connections;
        if (oc < nc) {
            printf("\nNew Connection: %d\n", (nc-oc));
            printf("Total Connections: %d\n\n", nc);
        } else if (oc > nc) {
            printf("\nConnection Disconnected: %d\n", (oc-nc));
            printf("Total Connections: %d\n\n", nc);
        }
        if(strcmp(od, data) != 0) {
            printf("Msg: %s\n",data);
            strcpy(od, data);
        }
    }
    shmctl(shmid, IPC_RMID, 0);
}

void client(key_t key) {
    int shmid;
    void *segment;
    int *connections;
    int *running;
    char *data;
    shmid = shmget(key, SHMSZ, IPC_CREAT|0666);
    if(shmid < 0) {
        fprintf(stderr, "Share segment creation failed!\n");
        exit(EXIT_FAILURE);
    }
    segment = shmat(shmid, NULL, 0);
    running = (int *) segment;
    if(! *running) {
        fprintf(stderr, "Server not running!\n");
        exit(EXIT_FAILURE);
    }
    connections = (int *) segment + sizeof(int);
    *connections++;
    printf("Currecnt Connections: %d\n", *connections);
    data = (char *) segment + (2 * sizeof(int));
    while(1) {
        printf("Msg: ");
        char *msg = (char *) malloc(768);
        scanf("%s", msg);
        strcpy(data, msg);
    }
}

int main(int argc, const char *argv[]) {
    int shmid, pid;

    if(argc != 3) {
        fprintf(stderr, "Usage: %s <cmd> <key>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int key;
    sscanf(argv[2], "%d", &key);

    if (strcmp(argv[1], "server") == 0) {
        server((key_t) key);
    } else if (strcmp(argv[1], "client") == 0) {
        client((key_t) key);
    } else {
        fprintf(stderr, "Invalid command!\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}
