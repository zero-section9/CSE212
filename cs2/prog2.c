//
//  prog2_2.c
//
//  Created by Abhishek Munie on 24/03/13.
//  Copyright (c) 2013 AbhishekMunie. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutexS1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexS2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexS3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexA = PTHREAD_MUTEX_INITIALIZER;

char *ingredients[3] = {"tobacco", "paper", "matches"};

int ai1;
int ai2;

int random_0_2() {
    return 3 * (rand() / (RAND_MAX + 1.0));
}

void *runS1() {
    while (1) {
        pthread_mutex_lock(&mutexS1);
        printf("Smoker 1 has %s, so he makes and smokes a cigarette.\n\n", ingredients[1]);
        pthread_mutex_unlock(&mutexA);
    }
}

void *runS2() {
    while (1) {
        pthread_mutex_lock(&mutexS2);
        printf("Smoker 2 has %s, so he makes and smokes a cigarette.\n\n", ingredients[0]);
        pthread_mutex_unlock(&mutexA);
    }
}

void *runS3() {
    while (1) {
        pthread_mutex_lock(&mutexS3);
        printf("Smoker 3 has %s, so he makes and smokes a cigarette.\n\n", ingredients[2]);
        pthread_mutex_unlock(&mutexA);
    }
}

void *runA() {
    while (1) {
        pthread_mutex_lock(&mutexA);
        ai1 = random_0_2();
        ai2 = random_0_2();
        if (ai1 == ai2) {
            ai2 = (ai1 + 2) % 3;
        }
        printf("Agent Places %s and %s on the table.\n", ingredients[ai1], ingredients[ai2]);
        if ((ai1 == 0 && ai2 == 2 ) || (ai1 == 2 && ai2 == 0 )) {
            pthread_mutex_unlock(&mutexS1);
        } else if ((ai1 == 1 && ai2 == 2 ) || (ai1 == 2 && ai2 == 1 )) {
            pthread_mutex_unlock(&mutexS2);
        } else {
            pthread_mutex_unlock(&mutexS3);
        }
    }
}

int main(int argc, const char * argv[]) {
    int rS1, rS2, rS3, rA;
    pthread_t threadS1, threadS2, threadS3, threadA;
    pthread_mutex_lock(&mutexS1);
    pthread_mutex_lock(&mutexS2);
    pthread_mutex_lock(&mutexS3);
    if ((rS1=pthread_create(&threadS1, NULL, runS1, NULL))) {
       printf("ThreadS1 creation failed: %d\n", rS1);
    }
    if ((rS2=pthread_create(&threadS2, NULL, runS2, NULL))) {
        printf("ThreadS2 creation failed: %d\n", rS2);
    }
    if ((rS3=pthread_create(&threadS3, NULL, runS3, NULL))) {
        printf("ThreadS3 creation failed: %d\n", rS3);
    }
    if ((rA=pthread_create(&threadA, NULL, runA, NULL))) {
        printf("ThreadA creation failed: %d\n", rA);
    }
    pthread_join(threadS1, NULL);
    pthread_join(threadS2, NULL);
    pthread_join(threadS3, NULL);
    pthread_join(threadA, NULL);
    return 0;
}