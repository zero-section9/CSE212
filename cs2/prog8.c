//
//  prog8.c
//  cse212
//
//  Created by Abhishek Munie on 06/05/13.
//  Copyright (c) 2013 AbhishekMunie. All rights reserved.
//

#include <stdio.h>
#include <limits.h>

int rq[] = {98, 183, 37, 122, 14, 124, 65, 67};
int len = 8;


void scheduleLOOK(int start) {
    int si = start;
    int i, j, nm, nmp;
    for (i = 0; i < len; i++) {
        nm = INT_MAX;
        do {
            for (j = i; j < len; j++) {
                if (rq[j] > si && rq[j] < nm) {
                    nm = rq[j];
                    nmp = j;
                }
            }
            if (nm == INT_MAX) {
                si = 0;
            } else {
                for (j = nmp; j > i; j--) {
                    rq[j] = rq[j - 1];
                }
                rq[i] = nm;
                si = nm;
            }
        } while (si == 0);
    }
}

int main() {
    int i;

    scheduleLOOK(53);
    for (i = 0; i < len; i++) {
        printf("%d ",rq[i]);
    }

    return 0;
}