#include "include/p2.h"
#include "include/structs.h"

#include <string.h>
#include <stdio.h>

#define FRAM_SIZE 256
#define ADDR_SIZE 28
#define OFFS_SIZE 8
#define FILE_ADDR "data.bin"

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

// Lee FRAM_SIZE bytes de data.bin y los copia en buffer
int read_disk(char* buffer[], unsigned int addr) {
    FILE* arch = NULL;
    char buff[256];
    memset(buff, 0, sizeof(buff)); // Inicializar vacio

    arch = fopen(FILE_ADDR, "rb");

    if(NULL == arch)
    {
        printf("\n fopen() Error!!!\n");
        return 1;
    }

    // printf("\n File opened successfully through fopen()\n");

    if(0 != fseek(arch, addr, SEEK_SET))
    {
        printf("\n fseek() failed\n");
        return 1;
    }

    // printf("\n fseek() successful\n");

    if(FRAM_SIZE != fread(buff, 1, FRAM_SIZE, arch)) {
        printf("\n fread() failed\n");
        return 1;
    }

    // printf("\n Some bytes successfully read through fread()\n");

    printf("\n The bytes read are [%s]\n",buff);

    fclose(arch);

    strcpy(*buffer, buff);

    // printf("\n File stream closed through fclose()\n");

    return 0;
}