#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

typedef struct {
  int count;
  pthread_mutex_t mutex;
} Semaphore;

void *startPA();
void *startPB();
void *startPC();
void *startC();

void wait(Semaphore *s)  {
    if(s->count <= 0) {
        pthread_mutex_lock(&(s->mutex));
    }
    s->count--;
}

void signal(Semaphore *s) {
    s->count++;
    if(s->count > 0) {
        pthread_mutex_unlock(&(s->mutex));
    }
}

Semaphore si = {-1, PTHREAD_MUTEX_INITIALIZER};
Semaphore siib = {1, PTHREAD_MUTEX_INITIALIZER};
Semaphore siic = {0, PTHREAD_MUTEX_INITIALIZER};
Semaphore siii = {0, PTHREAD_MUTEX_INITIALIZER};
Semaphore sC = {0, PTHREAD_MUTEX_INITIALIZER};

char buffer[1024];
int br;
int bw;
void write(char ch) {
    buffer[bw] = ch;
    signal(&sC);
    bw = (bw + 1) % 1024;
}

int main() {
    pthread_t tPA, tPB, tPC, tC;
    int retPA, retPB, retPC, retC;
    
    
    if(retPA = pthread_create(&tPA, NULL, startPA, NULL)) {
        printf("Thread PA creation failed: %d", retPA);
    }
    if(retPB = pthread_create(&tPB, NULL, startPB, NULL)) {
        printf("Thread PA creation failed: %d", retPB);
    }
    if(retPC = pthread_create(&tPC, NULL, startPC, NULL)) {
        printf("Thread PA creation failed: %d", retPC);
    }
    if(retC = pthread_create(&tC, NULL, startC, NULL)) {
        printf("Thread PA creation failed: %d", retC);
    }
    printf("The thread returns: %d", ret);
    pthread_join(tPA, NULL);
    pthread_join(tPB, NULL);
    pthread_join(tPC, NULL);
    pthread_join(tC, NULL);
    return 0;
}

void *startPA() {
    write('a');
    signal(&si);
    signal(&siii);
    while (true) {
        write('a');
        signal(&siii);
    }
}

void *startPB() {
    wait(&siii);
    write('b');
    wait(&siib);
    signal(&si);
    signal(&siic);
    while (true) {
        wait(&siib);
        wait(&siii);
        write('b');
        signal(&siic);
    }
}

void *startPC() {
    wait(&si);
    while (true) {
        wait(&siic);
        wait(&siii);
        write('c');
        signal(&siib);
    }
}

void *startC() {
    while(true) {
        wait(&sC);
        write(stdin, buffer+br, 1);
        br = (br + 1) % 1024;
    }
}
