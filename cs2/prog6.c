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

FreeNode *freeNodes = NULL;
AllocNode *allocNodes = NULL;

void addFreeNode(int start, int end) {
    FreeNode *nfn = NULL;
    FreeNode **fns = &freeNodes;

    while ((*fns != NULL) && ((*fns)->end < start)) {
        if ((*fns)->end == (start - 1)) {
            nfn = *fns;
            nfn-> end = end;
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

AllocNode *addAllocNode(int pid, int start, int end) {
    AllocNode **n = &allocNodes;
    AllocNode *nal = NULL;

    while((*n != NULL) && (((*n)->end < start))) {
        if(((*n)->end + 1) == start) {
            nal = *n;
            nal->end = end;
            break;
        }
        n = &((*n)->next);
    }

    if (nal == NULL) {
        nal = (AllocNode*) malloc(sizeof(AllocNode));
        nal->start = start;
        nal->end = end;
        nal->pid = pid;
        nal->next = *n;
        (*n) = nal;
    }

    if (nal->next != NULL &&(nal->end + 1) == nal->next->start) {
        nal->end = nal->next->end;
        AllocNode *antbd = nal->next;
        nal->next = nal->next->next;
        free(antbd);
    }

    return nal;
}

int finProcess(int pid) {
    AllocNode **n = &allocNodes;

    while(*n != NULL) {
        if ((*n)->pid == pid) {
            addFreeNode((*n)->start, (*n)->end);

            AllocNode *antbf = *n;
            (*n) = (*n)->next;
            free(antbf);

            return 0;
        }
        n = &((*n)->next);
    }
    return 1;
}

AllocNode *allocMem(FreeNode **fn, int size, int pid) {
    AllocNode *nal = NULL;

    int fns = (*fn)->end - (*fn)->start + 1;
    if (fns == size) {
        nal = addAllocNode(pid, (*fn)->start, (*fn)->start + size - 1);
        FreeNode *fntbd = *fn;
        (*fn) = (*fn)->next;
        free(fntbd);
    } else if (fns > size) {
        nal = addAllocNode(pid, (*fn)->start, (*fn)->start + size - 1);
        (*fn)->start = (*fn)->start + size;
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
    FreeNode **bfn = NULL;
    int bfs = INT_MAX;

    while (*fn != NULL) {
        int cs = (*fn)->end - (*fn)->start + 1;
        if (size <= cs && cs < bfs) {
            bfn = fn;
            bfs = cs;
        }
        fn = &((*fn)->next);
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
        fn = &((*fn)->next);
    }

    return (wfn != NULL) ? allocMem(wfn, size, pid) : NULL;
}

#define MEM_SIZE_VAL 1024
#define MEM_CW_VAL 50

#define ALLOC_CHAR '#'//219
#define FREE_CHAR ' '
#define UNKNOWN_CHAR '~'//176
#define END_CHAR '$'

void printMemView(int ch, int mi, int end) {
    for (; mi <= end; mi++) {
        if(mi % MEM_CW_VAL == 0) {
            printf("\n%5d ",MEM_CW_VAL * (mi/MEM_CW_VAL));
        }
        printf("%c",ch);
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
    //    char tick[] = {195, 169, 169, 169, 169, 169, 169, 169, 169, 169, 0};
    char tick[] = "|---------";
    for (i = 0; i < MEM_CW_VAL; i += 10) {
        printf("%s", tick);
    }
    while (mi < MEM_SIZE_VAL) {
        if (an != NULL && mi == an->start) {
            printMemView(ALLOC_CHAR, mi, an->end);
            mi = an->end + 1;
            an = an->next;
        } else if (fn != NULL && mi == fn->start) {
            printMemView(FREE_CHAR, mi, fn->end);
            mi = fn->end + 1;
            fn = fn ->next;
        } else {
            printMemView(UNKNOWN_CHAR, mi, mi);
            mi++;
        }
    }
    printf("%c\n",END_CHAR);
}

int main() {
    int fns[][2] = {
        //Start,  End, Next
        { 100,  199},
        { 345,  449},
        { 660,  670},
        { 910,  929},
        { 990, 1023}
    };
    int fnn = 5;

    int ans[][3] = {
        //PID,Start,  End, Next
        {  12,    0,   99},
        { 107,  200,  344},
        { 333,  450,  659},
        { 671,  671,  909},
        {1221,  930,  989}
    };
    int ann = 5;

    int i;
    for (i = 0; i < fnn; i++) {
        addFreeNode(fns[i][0], fns[i][1]);
    }
    for (i = 0; i < ann; i++) {
        addAllocNode(ans[i][0], ans[i][1], ans[i][2]);
    }
    
    printMemory();
    allocWorstFit(10, 191);
    printMemory();
    finProcess(333);
    printMemory();
    
    printf("\n");
    return 0;
}
