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

    // Crear Memoria
    Memory* mem = memory_init();

    // Crear TLB
    TLB* tlb = tlb_init();

    // Crear Page Directory inicial
    PageDirectory* pd = page_directory_init(1, size);

    char* filename = argv[2];
    char* file_buffer = get_buffer(filename);

    char* disk_buffer = (char*)malloc(FRAM_SIZE * sizeof(char));

    char* ch;
    char* dato;
    int start = 8; // Quitar el offset
    unsigned addr;
    unsigned offset;
    unsigned addr_wo_offset; // address without offset
    unsigned tlb_frame;
    unsigned pt_frame;

    void* current_table; // La tabla de cada iteracion
    current_table = pd;

    while ((ch = strsep(&file_buffer,"\n")) != NULL) {
        addr = atoi(ch);
        unsigned addresses[5] = {0}; // Desglose de la direccion
        offset = get_n_bits(addr, OFFS_SIZE, 0);
        addr_wo_offset = get_n_bits(addr, ADDR_SIZE - OFFS_SIZE, OFFS_SIZE);
        for (int i=4; i>=0; i--){ // Obtener desglose
            addresses[i] = get_n_bits(addr, size[i], start);
            start += size[i];
        }
        // Incrementar todos los timestamps
        tlb_incr_timestamps(tlb);
        mem_incr_timestamps(mem);

        tlb_frame = tlb_get_frame(tlb, addr_wo_offset);
        
        if (tlb_frame == TLB_MISS) { // Ir a buscarlo a la page table
            for (int nivel=1; nivel < niveles; nivel++) {
                if (nivel == niveles-1) { // Entra a Page Table 
                    PTE* pte = ((PageTable*)current_table)->entries[addresses[nivel]];
                    pt_frame = page_table_get_frame(current_table, addresses[nivel]);
                    if (pt_frame == PAGE_FAULT) { // Ir al disco
                        read_disk(&disk_buffer, addr - offset); // Para que quede de 28 bits
                        if (mem->is_full) { // Memoria llena
                            swap(mem, disk_buffer, pte);
                            tlb_set(tlb, addr_wo_offset, pte->frame);
                        }
                        else { // Memoria no llena
                            for (int i=0; i < RAM_SIZE; i++) {
                                ME* me = mem->frames[i];
                                if (me->data == NULL) { // Entrada vacia
                                    pte->frame = i; // Marcar la page table entry
                                    pte->obsol_bit = true; 

                                    // me->referrer->obsol_bit = false; // Indicar que ya no apunta al dato que apuntaba antes
                                    me->data = disk_buffer;
                                    me->referrer = pte;

                                    if (i == RAM_SIZE - 1) mem->is_full = true; // Se lleno la memoria
                                    tlb_set(tlb, addr_wo_offset, i);
                                    break;
                                }
                            }
                        }
                    }
                    else { // Frame obtenido de page table
                        printf("Page table hit\n");
                        dato = mem_get_data(mem, pt_frame);
                        printf("Dato: %d\n", dato[offset]);

                    }
                }
                else { // Entra a page directory
                    PDE* pde = pd->entries[addresses[nivel]];
                    if (pde->ptr == NULL) { // Si no ha sido inicializada
                        if (nivel == niveles - 2) pde->ptr = page_table_init(nivel, size);
                        else pde->ptr = page_table_init(nivel, size);
                    }
                    current_table = pde->ptr;
                }
            }
        }
        else { // Frame obtenido de la TLB
            printf("TLB hit\n");
            dato = mem_get_data(mem, tlb_frame);
            printf("Dato: %d\n", dato[offset]);
        }
    }

    // Liberar memoria
    free(file_buffer);
    free(disk_buffer);
}


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

    if(0 != fseek(arch, addr, SEEK_SET))
    {
        printf("\n fseek() failed\n");
        return 1;
    }

    if(FRAM_SIZE != fread(buff, 1, FRAM_SIZE, arch)) {
        printf("\n fread() failed\n");
        return 1;
    }

    // printf("The bytes read are [%s]\nAnd has size: %lu\n",buff, sizeof(buff));

    fclose(arch);
    strcpy(*buffer, buff);

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