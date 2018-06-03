#include "include/optimize.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int min_mem = 0;
int niveles[5] = {0};
int minimos[6] = {0};

int* optimize(int n, int bits, int mem_sum) {
    int x_mem = 0;
    int tot_mem = 0;

    if (n == 1) {
        if (bits == N_BITS) {
            tot_mem = mem_sum + (int)pow(2, bits) * 11;
        }
        else {
            tot_mem = mem_sum + (int)pow(2, bits) * niveles[1];
        }
    
        if (tot_mem < min_mem || min_mem == 0) {
            min_mem = tot_mem;
            niveles[n-1] = bits;
            for (int i=0; i < 5; i++) {
                minimos[i] = niveles[i];
            }
        }
        minimos[-1] = min_mem;
        return minimos;
    }

    for (int x=1; x < bits; x++) {
        niveles[n-1] = x;
        if (bits == N_BITS) {
            x_mem =  (int)pow(2, x) * 11;
        }
        else {
            x_mem =  (int)pow(2, x) * niveles[n];
        }
        optimize(n-1, bits-x, mem_sum + x_mem);
    }
    return minimos;
}
