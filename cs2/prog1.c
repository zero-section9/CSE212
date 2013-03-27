//
//  prog2_2.c
//
//  Created by Abhishek Munie on 24/03/13.
//  Copyright (c) 2013 AbhishekMunie. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUF_SIZE 1000

typedef struct {
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t condition_var;
} Semaphore;

pthread_mutex_t mutexP = PTHREAD_MUTEX_INITIALIZER;

Semaphore siA =  {0,        PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};
Semaphore siB =  {0,        PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};
Semaphore siiB = {0,        PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};
Semaphore siiC = {0,        PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};
Semaphore siii = {0,        PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};
Semaphore sP =   {BUF_SIZE, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};
Semaphore sC =   {0,        PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};

char buf[BUF_SIZE];
int ri = 0;
int wi = 0;

/*
void waitSemaphore(Semaphore *s) {
    if (s->count >= 0) {
        s->count--;
    }
    while(s->count < 0);
}
void signalSemaphore(Semaphore *s) {
    s->count++;
}*/

void waitSemaphore(Semaphore *s) {
    pthread_mutex_lock(&(s->mutex));
    if (s->count >= 0) {
        s->count--;
    }
    if (s->count < 0) {
        pthread_cond_wait(&(s->condition_var), &(s->mutex));
    }
    pthread_mutex_unlock(&(s->mutex));
}
void signalSemaphore(Semaphore *s) {
    pthread_mutex_lock(&(s->mutex));
    s->count++;
    if (s->count >= 0) {
        pthread_cond_signal(&(s->condition_var));
    }
    pthread_mutex_unlock(&(s->mutex));
}

void produce(char ch) {
    pthread_mutex_lock(&mutexP);
    waitSemaphore(&sP);
    buf[wi++] = ch;
    wi %= BUF_SIZE;
    signalSemaphore(&sC);
    pthread_mutex_unlock(&mutexP);
}

void *runPA() {
    produce('A');
    signalSemaphore(&siA);
    signalSemaphore(&siii);
    while (1) {
        produce('A');
        signalSemaphore(&siii);
    }
}

void *runPB() {
    waitSemaphore(&siii);
    produce('B');
    signalSemaphore(&siB);
    signalSemaphore(&siiC);
    while (1) {
        waitSemaphore(&siiB);
        waitSemaphore(&siii);
        produce('B');
        signalSemaphore(&siiC);
    }
}

void *runPC() {
    waitSemaphore(&siA);
    waitSemaphore(&siB);
    while (1) {
        waitSemaphore(&siiC);
        waitSemaphore(&siii);
        produce('C');
        signalSemaphore(&siiB);
    }
}

void *runC() {
    while (1) {
        waitSemaphore(&sC);
        putchar(buf[ri++]);
        ri %= BUF_SIZE;
        signalSemaphore(&sP);
    }
}

int main(int argc, const char * argv[]) {
    int rPA, rPB, rPC, rC;
    pthread_t threadPA, threadPB, threadPC, threadC;
    if ((rPB=pthread_create(&threadPB, NULL, runPB, NULL))) {
        printf("ThreadPB creation failed: %d\n", rPB);
    }
    if ((rC=pthread_create(&threadC, NULL, runC, NULL))) {
        printf("ThreadC creation failed: %d\n", rC);
    }
    if ((rPC=pthread_create(&threadPC, NULL, runPC, NULL))) {
        printf("ThreadPC creation failed: %d\n", rPC);
    }
    if ((rPA=pthread_create(&threadPA, NULL, runPA, NULL))) {
       printf("ThreadPA creation failed: %d\n", rPA);
    }
    pthread_join(threadPA, NULL);
    pthread_join(threadPB, NULL);
    pthread_join(threadPC, NULL);
    pthread_join(threadC, NULL);
    return 0;
}