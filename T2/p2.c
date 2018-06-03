#include "include/p2.h"
#include "include/structs.h"
#include "include/optimize.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define FRAM_SIZE 256
#define ADDR_SIZE 28
#define OFFS_SIZE 8
#define DISK_ADDR "/home/iic2333/T2/data.bin"

int main(int argc, char *argv[]){
    printf("Inicio de la simulacion\n\n");

    int niveles = atoi(argv[1]);
    printf("Numero de niveles: %d\n", niveles);

    int* size; // Tamano de cada nivel
    size = optimize(niveles, N_BITS, 0);
    printf("Tamanos por nivel:\n");
    for (int i=0; i < 5; i++) {
        if (size[i] != 0) printf("Nivel %d: %d\n", i+1, size[i]);
    }
    printf("\nMemoria usada: %d bytes\n\n", size[-1]/8);

    // Crear Memoria
    Memory* mem = memory_init();

    // Crear TLB
    TLB* tlb = tlb_init();

    // Crear Page Directory (o table) inicial
    void* table;
    if (niveles == 1) table = page_table_init(0, size);
    else table = page_directory_init(0, size);

    // La tabla de cada iteracion
    void* current_table;

    char* filename = argv[2];
    char* file_buffer = get_buffer(filename);
    char* file_buffer_aux = file_buffer; // Para liberarlo luego

    char* disk_buffer = (char*)malloc(FRAM_SIZE * sizeof(char));

    char* ch;
    unsigned char dato;
    unsigned start; // Para desglosar la direccion
    unsigned addr;
    unsigned offset;
    unsigned addr_wo_offset; // address without offset
    unsigned frame;

    int counter          = 0;
    int tlb_hit_count    = 0;
    int page_fault_count = 0;

    while ((ch = strsep(&file_buffer,"\n")) != NULL) {
        addr = atoi(ch);
        unsigned addresses[5] = {0}; // Desglose de la direccion
        offset = get_n_bits(addr, OFFS_SIZE, 0);
        addr_wo_offset = get_n_bits(addr, ADDR_SIZE - OFFS_SIZE, OFFS_SIZE);

        start = OFFS_SIZE;
        for (int i=4; i>=0; i--){ // Obtener desglose
            addresses[i] = get_n_bits(addr, size[i], start);
            start += size[i];
        }

        // Incrementar todos los timestamps
        tlb_incr_timestamps(tlb);
        mem_incr_timestamps(mem);

        counter++;
        printf("-%d-\n", addr);

        // Siempre inicia desde table
        current_table = table;

        frame = tlb_get_frame(tlb, addr_wo_offset);
        
        if (frame == TLB_MISS) { // Ir a buscarlo a la page table
            for (int nivel=0; nivel < niveles; nivel++) {
                if (nivel == niveles-1) { // Entra a Page Table 
                    PTE* pte = ((PageTable*)current_table)->entries[addresses[nivel]];
                    frame = page_table_get_frame((PageTable*)current_table, addresses[nivel]);

                    if (frame == PAGE_FAULT) { // Ir al disco
                        page_fault_count++;
                        read_disk(&disk_buffer, addr - offset); // Para apuntar al inicio del frame
                        dato = disk_buffer[offset];

                        if (mem->is_full) { // Memoria llena
                            swap(mem, disk_buffer, pte);
                            tlb_set(tlb, addr_wo_offset, pte->frame);
                            frame = pte->frame;
                        }
                        else { // Memoria no llena
                            for (int i=0; i < MEM_SIZE; i++) {
                                ME* me = mem->frames[i];

                                if (me->data == NULL) { // Entrada vacia
                                    pte->frame = i; // Marcar la page table entry
                                    pte->obsol_bit = true; 

                                    me->data = disk_buffer;
                                    me->referrer = pte;

                                    if (i == MEM_SIZE - 1) mem->is_full = true; // Se lleno la memoria
                                    tlb_set(tlb, addr_wo_offset, i);
                                    frame = i;
                                    break;
                                }
                            }
                        }
                    }
                    else { // Frame obtenido de page table
                        dato = mem_get_data(mem, frame)[offset];
                    }
                }
                else { // Entra a page directory
                    PDE* pde = ((PageDirectory*)current_table)->entries[addresses[nivel]];
                    if (pde->ptr == NULL) { // Si no ha sido inicializada
                        if (nivel == niveles - 2) pde->ptr = page_table_init(nivel+1, size);
                        else pde->ptr = page_directory_init(nivel+1, size);
                    }
                    current_table = pde->ptr;
                }
            }
        }
        else { // Frame obtenido de la TLB
            tlb_hit_count++;
            dato = mem_get_data(mem, frame)[offset];
        }
        printf("Dato obtenido: %u\n", dato);
        printf("Memoria fisica: %u\n", (frame << OFFS_SIZE) + offset);
        printf("\n");
    }

    // Final prints
    printf("\nSimulacion terminada\n\nPorcentaje de TLB hits: %d%%\n\
Porcentaje de page faults: %d%%\n", (tlb_hit_count * 100) / counter, (page_fault_count * 100) / counter);
    printf("\nTLB\n%-5s", "i");
    for (int j=1; j <= niveles; j++){ // Obtener desglose
        printf("nivel_%-4d", j);
    }
    printf("frame\n");
    int address;
    for (int i=0; i < TLB_SIZE; i++) {
        printf("%-5d", i);
        address = tlb->entries[i]->page;
        start = 0;
        int addresses[5] = {0};

        for (int j=niveles-1; j >= 0; j--){ // Obtener desglose
            addresses[j] = get_n_bits(address, size[j], start);
            start += size[j];
        }

        for (int nivel = 0; nivel < niveles; nivel++) {
            printf("%-10d", addresses[nivel]);
        }
        printf("%-5d\n", tlb->entries[i]->frame);
    }

    // Liberar memoria
    free(file_buffer_aux);
    free(disk_buffer);
    tlb_destroy(tlb);
    mem_destroy(mem);
    table_destroy(table, 1, niveles);
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