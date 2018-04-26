#include "include/p2.h"
#include "include/structs.h"
#include "include/optimize.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define FRAM_SIZE 256
#define ADDR_SIZE 28
#define OFFS_SIZE 8
#define DISK_ADDR "data.bin"

int main(int argc, char *argv[]){
    printf("Inicio\n");

    int niveles = atoi(argv[1]);
    printf("Numero de niveles: %d\n", niveles);

    int* size; // Tamano de cada nivel
    size = optimize(niveles, N_BITS, 0);
    printf("Tamanos por nivel: ");
    for (int i=0; i < 5; i++) {
        if (size[i] != 0) printf("%d ", size[i]);
    }
    printf("\nMemoria usada: %d bits\n", size[-1]);

    char* filename = argv[2];
    char* file_buffer = get_buffer(filename);

    char* ch;
    int addr;
    while ((ch = strsep(&file_buffer,"\n")) != NULL) {
        addr = atoi(ch);
        int desglose[5]; // Desglose de la direccion
        int start = 8;
        int offset = get_n_bits(addr, 8, 0);
        for (int i=4; i>=0; i--){ // Obtener desglose
            desglose[i] = get_n_bits(addr, size[i], start);
            start += size[i];
        }
    }

    int desglose[5]; // Desglose de la direccion
    int start = 8;
    int offset = get_n_bits(123456789, 8, 0);
    for (int i=4; i>=0; i--){ // Obtener desglose
        desglose[i] = get_n_bits(123456789, size[i], start);
        start += size[i];
    }
    printf("Offset: %d\n", offset);
    printf("Desglose por nivel: ");
    for (int i=0; i < 5; i++) {
        if (desglose[i] != 0) printf("%d ", desglose[i]);
    }
    printf("\n");



    free(file_buffer);
    char* buffer = (char*)malloc(FRAM_SIZE * sizeof(char));
    read_disk(&buffer, 0);

    TLB* tlb = tlb_init();
    printf("TLB creada\n");
    printf("Clock de tlb: %d\n", tlb->clock);
    printf("Timestamp de primera TLBE: %d\n", tlb->entries[0].timestamp);

    PageDirectory* pd = page_directory_init(1, size);
    printf("Page directory creada\n");
    printf("Page del tercer PDE: %d\n", pd->entries[2].page);

    PageTable* pt = page_table_init(1, size);
    printf("Page table creada\n");
    printf("Page del segundo PTE: %d\n", pt->entries[1].page);

    free(buffer);
}

// unsigned int_to_bin(unsigned k) {
//     if (k == 0) return 0;
//     return (k % 2) + 10 * int_to_int(k / 2);
// }

// unsigned* parse_int(unsigned n) {
//     unsigned bin = int_to_bin(n);
// }

// Retorna n bits de num partiendo desde start (0-index, derecha a izq)
unsigned get_n_bits(unsigned num, unsigned n, unsigned start) {
    unsigned  mask;
    mask = ((1 << n) - 1) << start; // Creo n 1's, los muevo hasta start
    return (num & mask) >> start; // bitwise AND para dejar solo los bits que quiero
}

// Lee FRAM_SIZE bytes de data.bin y los copia en buffer
int read_disk(char* buffer[], unsigned addr) {
    FILE* arch = NULL;
    char buff[256];
    memset(buff, 0, sizeof(buff)); // Inicializar vacio

    arch = fopen(DISK_ADDR, "rb");

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

    //printf("The bytes read are [%s]\nAnd has size: %lu\n",buff, sizeof(buff));

    fclose(arch);
    strcpy(*buffer, buff);

    // printf("\n File stream closed through fclose()\n");

    return 0;
}

// Retorna el buffer completo del archivo
char* get_buffer(char filename[]){
    FILE    *infile;
    char    *buffer;
    long    numbytes;

    infile = fopen(filename, "r");
    if(infile == NULL)
    {
        return "Error";
    }
    //Lee numero de bytes del archivo
    fseek(infile, 0L, SEEK_END);
    numbytes = ftell(infile);

    //resetear el indicador de posision del file al principio del file
    fseek(infile, 0L, SEEK_SET);

    //Tomar la memoria necesaria para que el buffer contenga al texto
    buffer = (char*)calloc(numbytes, sizeof(char));

    //controlamos memory error
    if(buffer == NULL)
    return "Error";

    //Copiamos el texto al buffer
    fread(buffer, sizeof(char), numbytes, infile);
    fclose(infile);

    return buffer;
}