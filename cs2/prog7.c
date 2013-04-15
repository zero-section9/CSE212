#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int pn;
char page[100];
char pageRecords[100][101];

int sl;
char string[100];

int hits;

bool isInPage(char ch) {
    int i;
    for (i = 0; i < pn; i++) {
        if (page[i] == ch) {
            return true;
        }
    }
    return false;
}

void setHit(int j) {
    int i;
    for (i = 0; i < pn; i++) {
        pageRecords[i][j] = ' ';
    }
}

void setMiss(int j) {
    int i;
    for (i = 0; i < pn; i++) {
        pageRecords[i][j] = page[i];
    }
}

void setEnd(int j) {
    int i;
    for (i = 0; i < pn; i++) {
        pageRecords[i][j] = '\0';
    }
}

void pageFIFO() {
    int i, j;
    i = 0;
    hits = 0;
    for (j = 0; j < sl; j++) {
        if (isInPage(string[j])) {
            hits++;
            setHit(j);
        } else {
            page[i] = string[j];
            i = (i + 1) % pn;
            setMiss(j);
        }
    }
    setEnd(j);
}

int getLRU(int sj) {
    int i, j, f, p, lru;
    p = sj;
    for (i = 0; i < pn; i++) {
        f = false;
        for (j = sj-1; !f && j >= 0; j--) {
            if (string[j] == page[i]){
                f = true;
                if (j < p) {
                    lru = i;
                    p = j;
                }
            }
        }
        if (!f) {
            return i;
        }
    }
    return lru;
}

void pageLRU() {
    int i;
    i = 0;
    hits = 0;
    for (i = 0; i < sl; i++) {
        if (isInPage(string[i])) {
            hits++;
            setHit(i);
        } else {
            page[getLRU(i)] = string[i];
            setMiss(i);
        }
    }
    setEnd(i);
}

int getOptimal(int sj) {
    int i, j, f, p, optimal;
    p = sj;
    for (i = 0; i < pn; i++) {
        f = false;
        for (j = sj+1; !f && j < sl; j++) {
            if (string[j] == page[i]){
                f = true;
                if (j > p) {
                    optimal = i;
                    p = j;
                }
            }
        }
        if (!f) {
            return i;
        }
    }
    return optimal;
}

void pageOptimal() {
    int i;
    i = 0;
    hits = 0;
    for (i = 0; i < sl; i++) {
        if (isInPage(string[i])) {
            hits++;
            setHit(i);
        } else {
            page[getOptimal(i)] = string[i];
            setMiss(i);
        }
    }
    setEnd(i);
}

void printRecords() {
    int i;
    printf("%s\n", string);
    for (i = 0; i < pn; i++) {
        printf("%s\n", pageRecords[i]);
    }
    printf("\nNumber of Hits: %d\n", hits);
    printf("Number of Miss: %d\n", sl - hits);
    printf("Page-Fault Rate: %.2f\n\n", 100.0 * (sl - hits) / sl);
}

int main(int argc, const char *argv[]) {
    int i;

    printf("Enter the Page Size: ");
    scanf("%d", &pn);
    printf("Enter the String: ");
    scanf("%s", string);
    sl = strlen(string);

    for (i = 0; i < pn; i++) {
        page[i] = ' ';
    }
    pageFIFO();
    printf("\nFIFO Paging:\n");
    printRecords();

    for (i = 0; i < pn; i++) {
        page[i] = ' ';
    }
    pageLRU();
    printf("\nLRU Paging:\n");
    printRecords();

    for (i = 0; i < pn; i++) {
        page[i] = ' ';
    }
    pageOptimal();
    printf("\nOptimal Paging:\n");
    printRecords();

    printf("\n");
    return 0;
}