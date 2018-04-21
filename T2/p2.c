#include "include/p2.h"
#include "include/tlb.h"

int main(int argc, char *argv[]){
    TLB* tlb = tlb_init();
}

unsigned int int_to_bin(unsigned int k) {
    if (k == 0) return 0;
    return (k % 2) + 10 * int_to_int(k / 2);
}

unsigned int* parse_int(unsigned int n) {
    unsigned int bin = int_to_bin(n);
}