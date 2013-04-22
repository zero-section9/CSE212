#include <stdio.h>
#include <limits.h>


typedef struct {
    int start;
    int end;
    FreeNode *next;
} FreeNode;

typedef struct {
    int pid;
    int start;
    int end;
    AllocNode *next;
} AllocNode;

FreeNode *freeNodes;
AllocNode *allocNodes;

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

    while((*n != NULL) && ((*n->end < (*fn)->start))) {
        if(((*n)->end + 1) == (*fn)->start) {
            nal = *n 
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
        nal->end = nal->next->end
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
    FreeNode **bfn;
    int bfs = INT_MAX;

    while (*fn != NULL) {
        
    }

    return (bfn != NULL) ? allocMem(bfn, size, pid) : NULL;
}

int main() {
    
    return 0;
}
