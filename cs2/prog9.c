//
//  prog9.c
//  cse212
//
//  Created by Abhishek Munie on 06/05/13.
//  Copyright (c) 2013 AbhishekMunie. All rights reserved.
//

#include <stdio.h>
#include <limits.h>
#include <string.h>

typedef struct {
    char name[100];
    int start;
    int length;
} File;


File files[] = {
    {"count",  0, 2},
    {"tr",    14, 3},
    {"mail",  19, 6},
    {"list",  28, 4},
    {"f",      6, 2}
};
int fn;

#define DISK_SIZE 10240

typedef struct FreeNode {
    int start;
    int end;
    struct FreeNode *next;
} FreeNode;

FreeNode *freeNodes = NULL;

void addFreeNode(int start, int end) {
    FreeNode *nfn = NULL;
    FreeNode **fns = &freeNodes;

    while ((*fns != NULL) && ((*fns)->end < start)) {
        if ((*fns)->end == (start - 1)) {
            nfn = *fns;
            nfn->end = end;
            break;
        }
        fns = &((*fns)->next);
    }

    if (nfn == NULL) {
        nfn = (FreeNode*) malloc(sizeof(FreeNode));
        nfn->start = start;
        nfn->end = end;
        nfn->next = *fns;
        (*fns) = nfn;
    }

    if (nfn->next != NULL && (nfn->end + 1) == nfn->next->start) {
        nfn->end = nfn->next->end;
        FreeNode *fntbd = nfn->next;
        nfn->next = nfn->next->next;
        free(fntbd);
    }
}

File *allocFile(FreeNode **f, char *name, int size) {
    int fns = (*f)->end - (*f)->start + 1;
    if (fns == size) {
        FreeNode *fntbd = *f;
        (*f) = (*f)->next;
        free(fntbd);
    } else if (fns > size) {
        (*f)->start = (*f)->start + size;
    }

    if (fns >= size) {
        strcpy(files[fn].name, name);
        files[fn].start = (*f)->start;
        files[fn++].length = size;
        return &files[fn-1];
    }

    return NULL;
}


File *allocBestFit(char *name, int size) {
    FreeNode **fn = &freeNodes;
    FreeNode **bfn =  NULL;
    int bfs = INT_MAX;

    while (*fn != NULL) {
        int cs = (*fn)->end - (*fn)->start + 1;
        if (size <= cs && cs < bfs) {
            bfn = fn;
            bfs = cs;
        }
        fn = &((*fn)->next);
    }

    return (bfn != NULL) ? allocFile(bfn, name, size) : NULL;
}

int main() {

    return 0;
}