#include "include/structs.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

/** Crea una TLBE inicialmente vacia y retorna el puntero */
TLBE* tlbe_init() {
    TLBE* entry = malloc(sizeof(TLBE));
    entry->page      = 0;
    entry->frame     = 0;
    entry->timestamp = 0;
    return entry;
}

/** Crea una ME inicialmente vacia y retorna el puntero */
ME* me_init() {
    ME* entry = malloc(sizeof(ME));
    entry->data      = (char*)NULL;
    entry->referrer  = (PTE*)NULL;
    entry->timestamp = 0;
    return entry;
}

/** Crea una PTE inicialmente vacia y retorna el puntero */
PTE* pte_init() {
    PTE* entry = malloc(sizeof(PTE));
    entry->frame     = 0;
    entry->obsol_bit = false;
    return entry;
}

/** Crea una PDE inicialmente vacia y retorna el puntero */
PDE* pde_init() {
    PDE* entry = malloc(sizeof(PDE));
    entry->page = 0;
    entry->ptr  = NULL;
    return entry;
}

// Incrementa todos los timestamps de la TLB
void tlb_incr_timestamps(TLB* tlb) {
    for (int i=0; i < TLB_SIZE; i++) {
        TLBE* entry = tlb->entries[i];
        if (entry != NULL) entry->timestamp++;
    }
}

// Incrementa todos los timestamps de la memoria
void mem_incr_timestamps(Memory* mem) {
    for (int i=0; i < MEM_SIZE; i++) {
        ME* entry = mem->frames[i];
        if (entry != NULL) entry->timestamp++;
    }
}

/** Crea una TLB inicialmente vacia y retorna el puntero */
TLB* tlb_init() {
    TLB* tlb = (TLB*)malloc(sizeof(TLB));
    tlb->lru           = (TLBE*)NULL;
    tlb->max_timestamp = 0;
    tlb->is_full       = false;
    tlb->entries       = (TLBE**)calloc(TLB_SIZE, sizeof(TLBE*));

    for (int i = 0; i < TLB_SIZE; i++) tlb->entries[i] = tlbe_init();

    return tlb;
}

/** Retorna el frame del elemento en la pagina dada
 * Retorna TLB_MISS si no encuentra
*/
unsigned tlb_get_frame(TLB* tlb, unsigned page) {
    for (int i = 0; i < TLB_SIZE; i++){
        TLBE* node = tlb->entries[i];
        if (node->page == page) {
            node->timestamp = 0; // Resetear timestamp ya que se acaba de usar
            return node->frame;
        }
    }
    return TLB_MISS; // No se encontro en la TLB
}

// Setea la entrada LRU
void tlb_update_lru(TLB* tlb) {
    TLBE* node;
    for (int i = 0; i < TLB_SIZE; i++){
        node = tlb->entries[i];
        if (tlb->max_timestamp < node->timestamp) {
            tlb->max_timestamp = node->timestamp;
            tlb->lru = tlb->entries[i];
        }
    }
    tlb->max_timestamp = 0;
}

// Mete el frame a la TLB segun LRU
void tlb_set(TLB* tlb, unsigned page, unsigned frame) {
    TLBE* node;
    if (!tlb->is_full) { // Todavia se esta llenando la TLB
        for (int i = 0; i < TLB_SIZE; i++){
            node = tlb->entries[i];
            if (0 == node->page) { // TLBE vacia
                if (i == TLB_SIZE - 1) {
                    tlb->is_full = true; // Se lleno la TLB
                printf("TLB FULL\n");
                }
                break;
            }
        }
    }
    else { // If full, run LRU
        tlb_update_lru(tlb); // Set max_timestamp y lru
        node = tlb->lru;
    }

    node->page = page;
    node->frame = frame;
    node->timestamp = 0;
    return;
}

/** Libera todos los recursos asociados a la TLB */
void tlb_destroy(TLB* tlb) {
    for (int i = 0; i < TLB_SIZE; i++){
        free(tlb->entries[i]);
    }
    free(tlb->entries);
    free(tlb);
}

/** Crea una TLB inicialmente vacia y retorna el puntero */
Memory* memory_init() {
    Memory* mem = (Memory*)malloc(sizeof(Memory));
    mem->lru           = 0;
    mem->max_timestamp = 0;
    mem->is_full       = false;
    mem->frames = (ME**)calloc(MEM_SIZE, sizeof(ME*));
    
    for (int i = 0; i < MEM_SIZE; i++) mem->frames[i] = me_init();

    return mem;
}

// Setea la entrada LRU
void mem_update_lru(Memory* mem) {
    for (int i = 0; i < MEM_SIZE; i++){
        ME* node = mem->frames[i];
        if (mem->max_timestamp < node->timestamp) {
            mem->max_timestamp = node->timestamp;
            mem->lru = mem->frames[i];
        }
    }
    mem->max_timestamp = 0;
}

/* Mete la data en memoria, saca al LRU y actualiza la PT */
void swap(Memory* mem, char* data, PTE* pte) {
    mem_update_lru(mem);
    ME* lru = mem->lru;

    pte->frame = lru->referrer->frame;
    pte->obsol_bit = true; 

    lru->referrer->obsol_bit = false; // Indicar que ya no apunta al dato que apuntaba antes
    lru->data = data;
    lru->referrer = pte;
    lru->timestamp = 0;

    return;
}

// Retorna el dato de 256 bytes de un frame de memoria
char* mem_get_data(Memory* mem, unsigned frame) {
    return mem->frames[frame]->data;
}

/** Libera todos los recursos asociados a la Memoria */
void mem_destroy(Memory* mem) {
    for (int i = 0; i < MEM_SIZE; i++){
        free(mem->frames[i]);
    }
    free(mem->frames);
    free(mem);
}

// Obtiene el numero de frame asociado a una pagina, o page fault
unsigned page_table_get_frame(PageTable* pt, unsigned page) {
    PTE* pte = pt->entries[page];
    if (pte->obsol_bit) return pte->frame;
    else return PAGE_FAULT;
}

PageTable* page_table_init(unsigned level, int* size) { // level es en realidad el nivel-1
    PageTable* pt = (PageTable*)malloc(sizeof(PageTable));
    pt->entries = (PTE**)calloc((int)pow(2, size[level]), sizeof(PTE*));
    pt->size = (unsigned)pow(2, size[level]);
    // Inicializar entries
    for (int i = 0; i < pt->size; i++) pt->entries[i] = pte_init();
    return pt;
}

PageDirectory* page_directory_init(unsigned level, int* size) { // level es en realidad el nivel-1
    PageDirectory* pd = (PageDirectory*)malloc(sizeof(PageDirectory));
    pd->entries = (PDE**)calloc((int)pow(2, size[level]), sizeof(PDE*));
    pd->size = (unsigned)pow(2, size[level]);
    // Inicializar entries
    for (int i = 0; i < pd->size; i++) pd->entries[i] = pde_init();
    return pd;
}

// Libera todas las tablas recursivamente
void table_destroy(void* table, unsigned nivel, unsigned niveles) {
    if (nivel == niveles) { // es page table
        for (int i=0; i < ((PageTable*)table)->size; i++) {
            free(((PageTable*)table)->entries[i]);
        }
        free(((PageTable*)table)->entries);
        free((PageTable*)table);
    }
    else { // Es page directory
        for (int i=0; i < ((PageDirectory*)table)->size; i++) {
            PDE* entry = ((PageDirectory*)table)->entries[i];
            if (entry->ptr != NULL) { // Esta inicializada
                table_destroy(entry->ptr, nivel+1, niveles);
            }
            free(entry);
        }
        free(((PageDirectory*)table)->entries);
        free((PageDirectory*)table);
    }
}
