#include<sys/wait.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<errno.h>

void server(key_t key) {
    int shmid;
    int *connections, oc, nc;
    int *running;
    char *data, *od;
    od = (char *) malloc(768);
    printf("Starting Server... \n");
    shmid = shmget((key_t) key, 1024, IPC_CREAT|0660);
    if(shmid < 0) {
        fprintf(stderr, "Share segment creation failed!\n");
        exit(EXIT_FAILURE);
    }
    printf("Memory Mount Complete.\n");
    //running = (int *) shmat(shmid, (char *) 10, SHM_REMAP);
    //*running = 1;
    printf("Server started with key: %d\n=================================\n\n", key);
    connections = (int *) shmat(shmid, (char *) (12 + sizeof(int)), SHM_REMAP);
    printf("\n\n%d\n\n%d\n\n%d\n\n%d\n\n",errno, EACCES, EINVAL, ENOMEM);
    oc = *connections;
    printf("Connections: %d\n", oc);
    data = (char *) shmat(shmid, (char *) (15 + (2 * sizeof(int))), 0);
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
        if(strcmp(od, data) == 0) {
            printf(data);
            strcpy(od, data);
        }
    }
    shmctl(shmid, IPC_RMID, 0);
}

void client(key_t key) {
    int shmid;
    int *connections;
    int *running;
    char *data;
    shmid = shmget((key_t) key, 1024, IPC_CREAT|0660);
    if(shmid < 0) {
        fprintf(stderr, "Share segment creation failed!\n");
        exit(EXIT_FAILURE);
    }
    running = (int *) shmat(shmid, (char *) 10, 0);
    if(! *running) {
        fprintf(stderr, "Server not running!\n");
        exit(EXIT_FAILURE);
    }        
    connections = (int *) shmat(shmid, (char *) (12 + sizeof(int)), 0);
    printf("Currecnt Connections: %d\n", *connections);
    data = (char *) shmat(shmid, (char *) (15 + (2 * sizeof(int))), 0);
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
        server(key);
    } else if (strcmp(argv[1], "client") == 0) {
        client(key);
    } else {
        fprintf(stderr, "Invalid command!\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}
