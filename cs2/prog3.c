#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    int max[100];
    int allocation[100];
    int need[100];
    bool finished;
} Process;

int pn;
int rn;
int available[100];
int work[100];
Process processes[100];
int safeSeq[100];

void init() {
    int i, j;
    for (i = 0; i < rn; i++) {
        work[i] = available[i];
    }
    for (i = 0; i < pn; i++) {
        for (j = 0; j < rn; j++) {
            processes[i].need[j] = processes[i].max[j] - processes[i].allocation[j];
        }
        processes[i].finished = false;
    }
}

bool canProcessRun(int pi) {
    int i;
    for (i = 0; i < rn; i++) {
        if (processes[pi].need[i] > work[i]) {
            return false;
        }
    }
    return true;
}

void runProcess(int pi) {
    int i;
    for (i = 0; i < rn; i++) {
        work[i] += processes[pi].allocation[i];
    }
    processes[pi].finished = true;
}

bool isSafe() {
    int i, c;
    bool dsie;
    c = 0;
    do {
        dsie = false;
        for (i = 0; i < pn; i++) {
            if (!processes[i].finished && canProcessRun(i)) {
                dsie = true;
                safeSeq[c++] = i;
                runProcess(i);
            }
        }
    } while (dsie);
    return (c == pn);
}

int main(int argc, const char *argv[]) {
    int i;

    init();

    if (isSafe()) {
        printf("The System is in Safe State.\n");
        printf("Safe Seqence: <p%d", safeSeq[0]);
        for (i = 1; i < pn; i++) {
            printf(", p%d\n", safeSeq[i]);
        }
        printf(">\n");
    } else {
        printf("The Process is not in Safe State.\n");
    }

    return 0;
}