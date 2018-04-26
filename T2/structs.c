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
    entry->timestamp = 1;
    return entry;
}

/** Crea una TLB inicialmente vacia y retorna el puntero */
TLB* tlb_init() {
    TLB* tlb = (TLB*)malloc(sizeof(TLB));
    tlb->clock         = 1;
    tlb->lru           = 0;
    tlb->max_timestamp = 0;
    tlb->is_full       = false;
    tlb->entries = (TLBE*)calloc(TLB_SIZE, sizeof(TLBE));
    // Inicializar entries
    for (int i = 0; i < TLB_SIZE; i++) {
        TLBE entry = {
            .page      = 0,
            .frame     = 0,
            .timestamp = 1
        };
        tlb->entries[i] = entry;
    }
    // Inicializar entries
    // for (int i = 0; i < TLB_SIZE; i++) tlb->entries[i] = *tlbe_init();

    return tlb;
}

/** Retorna el frame del elemento en la pagina dada
 * Retorna TLB_MISS si no encuentra
*/
unsigned tlb_get_frame(TLB* tlb, unsigned page) {
    for (int i = 0; i < TLB_SIZE; i++){
        TLBE node = tlb->entries[i];
        if (node.page == page) {
            node.timestamp = 0; // Resetear timestamp ya que se acaba de usar
            return node.frame;
        }
    }
    return TLB_MISS; // No se encontro en la TLB
}

// Setea la entrada LRU
void tlb_update_lru(TLB* tlb) {
    for (int i = 0; i < TLB_SIZE; i++){
        TLBE node = tlb->entries[i];
        if (tlb->max_timestamp < node.timestamp) {
            tlb->max_timestamp = node.timestamp;
            tlb->lru = &(tlb->entries[i]);
        }
    }
}

// Mete el frame a la TLB segun LRU
void tlb_set(TLB* tlb, unsigned page, unsigned frame) {
    if (!tlb->is_full) { // Todavia se esta llenando la TLB
        for (int i = 0; i < TLB_SIZE; i++){
            TLBE node = tlb->entries[i];
            if (0 == node.page) {
                node.page = page;
                node.frame = frame;
                node.timestamp = 0;
                if (i == TLB_SIZE - 1) tlb->is_full = true; // Se lleno la TLB
                return;
            }
        }
    }

    // If full, run LRU

    tlb_update_lru(tlb); // Set max_timestamp y lru
    TLBE* node = tlb->lru;
    node->page = page;
    node->frame = frame;
    node->timestamp = 0;
    tlb_update_lru(tlb); // Reordenar
    return;
}

/** Libera todos los recursos asociados a la TLB */
void tlb_destroy(TLB* tlb) {
    for (int i = 0; i < TLB_SIZE; i++){
        free(&(tlb->entries[i]));
    }
    free(tlb);
}

// Setea la entrada LRU
void mem_update_lru(Memory* mem) {
    for (int i = 0; i < RAM_SIZE; i++){
        ME* node = mem->frames[i];
        if (mem->max_timestamp < node->timestamp) {
            mem->max_timestamp = node->timestamp;
            mem->lru = node;
        }
    }
}

void swap(Memory* mem, char* data, unsigned address) {
    ME* lru = mem->lru;
    //PTE* pte = get_page_table_entry(address);

    mem_update_lru(mem);
    lru->referrer->obsol_bit = false; // Indicar que ya no apunta al dato que apuntaba antes
    lru->data = data;
    lru->referrer = 0,//pte;
    lru->timestamp = 0;
    mem_update_lru(mem);
    return;
}

// PTE* get_page_table_entry(unsigned address) {

// }

// Quizas los inits() no tienen que crear un objeto sino recibirlo y poblarlo nomas.

PageTable* page_table_init(unsigned level, int* size) {
    PageTable* pt = (PageTable*)malloc(sizeof(PageTable));
    pt->entries = (PTE*)calloc((int)pow(2, size[level-1]), sizeof(PTE));
    // Inicializar entries
    for (int i = 0; i < (int)pow(2, size[level-1]); i++) {
        PTE pte = {.page=1, .frame=0, .valid_bit=false, .obsol_bit=false};
        pt->entries[i] = pte;
    }
    return pt;
}

PageDirectory* page_directory_init(unsigned level, int* size) {
    PageDirectory* pd = (PageDirectory*)malloc(sizeof(PageDirectory));
    pd->entries = (PDE*)calloc((int)pow(2, size[level-1]), sizeof(PDE));
    // Inicializar entries
    for (int i = 0; i < (int)pow(2, size[level-1]); i++) {
        PDE pde = {.page=0, .ptr=NULL};
        pd->entries[i] = pde;
    }
    return pd;
}


// page_table_init((int)pow(2, size[level]))