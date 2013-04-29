#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct FreeNode {
    int start;
    int end;
    struct FreeNode *next;
} FreeNode;

typedef struct AllocNode {
    int pid;
    int start;
    int end;
    struct AllocNode *next;
} AllocNode;

FreeNode *freeNodes;
AllocNode *allocNodes;

void addAllocNodes(AllocNode *an, int num) {
    int i;
    AllocNode **n = &allocNodes;
    while(n != NULL) {
        n = &((*n)->next);
    }
    (*n) = an;

    for (i = 1; i < num; i++) {
        (*n)->next = &an[i];
        n = &((*n)->next);
    }
}

void addAllocNode(AllocNode *an) {
    return addAllocNodes(an, 1);
}

void addFreeNodes(FreeNode *fn, int num) {
    int i;
    FreeNode **n = &freeNodes;
    while(n != NULL) {
        n = &((*n)->next);
    }
    (*n) = fn;

    for (i = 1; i < num; i++) {
        (*n)->next = &fn[i];
        n = &((*n)->next);
    }
}

void addFreeNode(FreeNode *fn) {
    return addFreeNodes(fn, 1);
}

int finProcess(int pid) {
    AllocNode **n = &allocNodes;
    
    while(*n != NULL) {
        if ((*n)->pid == pid) {
            FreeNode *nfn = NULL;
             
            FreeNode **fn = &freeNodes;
            while ((*fn == NULL) && ((*fn)->start > nfn->end)) {
                if ((*fn)->end == ((*n)->start - 1)) {
                    nfn = *fn;
                    nfn-> end = (*n)->end;
                    break;
                }
                fn = &((*fn)->next);
            }
            
            if (nfn == NULL) {
                nfn = (FreeNode*) malloc(sizeof(FreeNode));
                nfn->start = (*n)->start;
                nfn->end = (*n)->end;
                nfn->next = *fn;
                (*fn) = nfn;
            }

            AllocNode *antbf = *n;
            (*n) = (*n)->next;
            free(antbf);
            
            if ((nfn->end + 1) == nfn->next->start) {
                nfn->end = nfn->next->end;
                FreeNode *fntbd = nfn->next;
                nfn->next = nfn->next->next;
                free(fntbd);
            }
            
            return 0;
        }
        n = &((*n)->next);
    }
    return 1;
}

AllocNode *allocMem(FreeNode **fn, int size, int pid) {
    AllocNode** n = &allocNodes;
    
    AllocNode* nal = NULL;

    while((*n != NULL) && (((*n)->end < (*fn)->start))) {
        if(((*n)->end + 1) == (*fn)->start) {
            nal = *n; 
            nal->end = (*fn)->end;
            break;
        }
        n = &((*n)->next);
    }
    
    if (nal == NULL) {
        nal = (AllocNode*) malloc(sizeof(AllocNode));
        nal->start = (*fn)->start;
        nal->end = (*fn)->start + size - 1;
        nal->pid = pid;
        nal->next = *n;
        (*n) = nal;
    }
    
    if ((nal->end + 1) == nal->next->start) {
        nal->end = nal->next->end;
        AllocNode *antbd = nal->next;
        nal->next = nal->next->next;
        free(antbd);
    }
    
    int fns = (*fn)->end - (*fn)->start + 1;
    if (fns == size) {
        FreeNode *fntbd = *fn;
        (*fn) = (*fn)->next;
        free(fntbd);
    } else if (fns > size) {
        (*fn)->start = (*fn)->start + size;
    } else {
        
    }
    
    return nal;
}

AllocNode *allocFirstFit(int size, int pid) {
    FreeNode **fn = &freeNodes;
    while (*fn != NULL) {
        if (size <= ((*fn)->end - (*fn)->start + 1)) {
            return allocMem(fn, size, pid);
        }
    }
    return NULL;
}

AllocNode *allocBestFit(int size, int pid) {
    FreeNode **fn = &freeNodes;
    FreeNode **bfn =  NULL;
    int bfs = INT_MAX;

    while (*fn != NULL) {
       int cs = (*fn)->end - (*fn)->start + 1;
       if (size <= cs && cs < bfs) { 
           bfn = fn;
           bfs = cs;
       }
    }

    return (bfn != NULL) ? allocMem(bfn, size, pid) : NULL;
}

AllocNode *allocWorstFit(int size, int pid) {
    FreeNode **fn = &freeNodes;
    FreeNode **wfn = NULL;
    int wfs = 0;

    while (*fn != NULL) {
       int cs = (*fn)->end - (*fn)->start + 1;
       if (size <= cs && cs > wfs) {
           wfn = fn;
           wfs = cs;
       }
    }

    return (wfn != NULL) ? allocMem(wfn, size, pid) : NULL;
}

#define MEM_SIZE_VAL 1024
#define MEM_CW_VAL 50

void printMemView(int ch, int mi, int end) {
    for (; mi <= end; mi++) {
        putchar(ch);
        if(mi % MEM_CW_VAL == 0) {
            printf("\n");
        }
    }
}

void printMemory() {
    int i, mi = 0;
    FreeNode *fn = freeNodes;
    AllocNode *an = allocNodes;
    printf("      ");
    for (i = 0; i < MEM_CW_VAL; i += 10) {
        printf("%-10d",i);
    }
    printf("\n      ");
    char tick[] = {195, 169, 169, 169, 169, 169, 169, 169, 169, 169, 0};
    for (i = 0; i < MEM_CW_VAL; i += 10) {
        printf("%s", tick);
    }
    while (mi < MEM_SIZE_VAL) {
        if (mi == an->start) {
            printMemView(219, mi, an->end);
            mi = an->end + 1;
            an = an->next;
        } else if (mi == fn->start) {
            printMemView(' ', mi, fn->end);
            mi = fn->end + 1;
            fn = fn ->next;
        } else {
            printMemView(176, mi, mi);
            mi++;
        }
    }
}

int main() {
    FreeNode fns[] = {
        { 100,  199, NULL},
        { 345,  449, NULL},
        { 660,  670, NULL},
        { 910,  929, NULL},
        { 990, 1023, NULL}
    };
    int fnn = 5;
    AllocNode ans[] = {
        {  12,    0,   99, NULL},
        { 107,  200,  344, NULL},
        { 333,  450,  659, NULL},
        { 671,  671,  909, NULL},
        {1221,  930,  989, NULL}
    };
    int ann = 5;
    
    addFreeNodes(fns, fnn);
    addAllocNodes(ans, ann);
    
    printMemory();

    printf("\n");
    return 0;
}
