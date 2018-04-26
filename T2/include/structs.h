#ifndef TLB_H
#define TLB_H

#include <stdbool.h>

#define RAM_SIZE 256
#define TLB_SIZE 64
#define TLB_MISS -1

#define TYPE_PD 1 // PageDirectory
#define TYPE_PT 2 // PageTable

/******************** TLB ********************/

// TLB_Entry
typedef struct {
    unsigned page;
    unsigned frame;
    unsigned timestamp;
} TLBE;

/** Crea una TLB_Entry inicialmente vacia y retorna el puntero */
TLBE* tlbe_init();

// TLB
typedef struct {
    TLBE* entries; // Tendra tamano TLB_SIZE
    bool is_full;
    unsigned clock;
    TLBE* lru;
    unsigned max_timestamp;
} TLB;

/** Crea una TLB inicialmente vacia y retorna el puntero */
TLB* tlb_init();

/** Retorna el frame del elemento en la posicion dada */
unsigned tlb_get_frame(TLB* tlb, unsigned page);

// Mete el frame a la TLB segun LRU
void tlb_update_lru(TLB* tlb);

// Mete el frame a la TLB segun LRU
void tlb_set(TLB* tlb, unsigned page, unsigned frame);

/** Libera todos los recursos asociados a la TLB */
void tlb_destroy(TLB* tlb);

/***************** PAGE TABLE ****************/
/* Tabla que apunta a memoria fisica */

// PageTableEntry
typedef struct {
    unsigned page;
    unsigned frame; // 0 - 255
    bool valid_bit; // Bit de validez: 1 cuando ya se inicializo
    bool obsol_bit; // Bit de obsolesencia: 1 cuando apunta a un frame que tiene la info correspondiente.
} PTE;

typedef struct {
    PTE* entries;
} PageTable;

PTE* get_page_table_entry(unsigned address);

PageTable* page_table_init(unsigned level, int* size);

/************** PAGE DIRECTORY ***************/
/* Tabla que apunta a page tables */

// PageDirectoryEntry
typedef struct {
    unsigned page;
    void* ptr; // Puede ser PageTable o PageDirectory
} PDE;

typedef struct {
    PDE* entries;
} PageDirectory;

PageDirectory* page_directory_init(unsigned level, int* size);

/******************** RAM ********************/

// MemoryEntry
typedef struct {
    char* data;
    PTE* referrer; // PTE que esta apuntando a este frame
    unsigned timestamp;
} ME;

typedef struct {
    ME* frames[RAM_SIZE];
    bool is_full;
    unsigned clock;
    ME* lru;
    unsigned max_timestamp;
} Memory;

void mem_update_lru(Memory* mem);

void swap(Memory* mem, char* data, unsigned address);

#endif