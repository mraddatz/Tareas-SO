#ifndef TLB_H
#define TLB_H

#include <stdbool.h>

#define RAM_SIZE 256
#define TLB_SIZE 64
#define TLB_MISS -1

/******************** TLB ********************/

// TLB_Entry
typedef struct {
    unsigned int page;
    unsigned int frame;
    unsigned int timestamp;
} TLB_Entry;

/** Crea una TLB_Entry inicialmente vacia y retorna el puntero */
TLB_Entry* tlb_entry_init();

// TLB
typedef struct {
    TLB_Entry* entries[TLB_SIZE];
    bool is_full;
    unsigned int clock;
    TLB_Entry* lru;
    unsigned int max_timestamp;
} TLB;

/** Crea una TLB inicialmente vacia y retorna el puntero */
TLB* tlb_init();

/** Retorna el frame del elemento en la posicion dada */
unsigned int tlb_get_frame(TLB* tlb, unsigned int page);

// Mete el frame a la TLB segun LRU
void tlb_update_lru(TLB* tlb);

// Mete el frame a la TLB segun LRU
void tlb_set(TLB* tlb, unsigned int page, unsigned int frame);

/** Libera todos los recursos asociados a la TLB */
void tlb_destroy(TLB* tlb);

/******************** RAM ********************/

// RAM_Entry
typedef struct {
    char* data;
    unsigned int address; // Direccion virtual que esta apuntando a este frame
    unsigned int timestamp;
} RAM_Entry;

typedef struct {
    RAM_Entry* frames[RAM_SIZE];
    bool is_full;
    unsigned int clock;
    RAM_Entry* lru;
    unsigned int max_timestamp;
} Memory;

void mem_update_lru(Memory* mem);

/***************** PAGE TABLE ****************/

typedef struct {
    unsigned int page;
    unsigned int frame;
    bool valid_bit; // Bit de validez: 1 cuando ya se inicializo
    bool obsol_bit; // Bit de obsolesencia: 1 cuando apunta a un frame que tiene la info correspondiente.
} PageTableEntry;

typedef struct {
    PageTableEntry* entries;
} PageTable;

PageTable* page_table_init(unsigned int size);

void obsoletize_page(PageTable* pt, unsigned int address);

void swap(PageTable* pt, Memory* mem, char* data, unsigned int address);

#endif