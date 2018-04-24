#include "include/structs.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/** Crea una TLB_Entry inicialmente vacia y retorna el puntero */
TLB_Entry* tlb_entry_init() {
    TLB_Entry *entry = malloc(sizeof(TLB_Entry));
    entry->page      = 0;
    entry->frame     = 0;
    entry->timestamp = 0;

    return entry;
}

/** Crea una TLB inicialmente vacia y retorna el puntero */
TLB* tlb_init() {
    TLB *tlb; // = malloc(sizeof(TLB));
    tlb->clock         = 0;
    tlb->lru           = 0;
    tlb->max_timestamp = 0;
    tlb->is_full       = false;

    // Inicializar entries
    for (int i = 0; i < TLB_SIZE; i++) tlb->entries[0] = tlb_entry_init();

    return tlb;
}

/** Retorna el frame del elemento en la pagina dada
 * Retorna TLB_MISS si no encuentra
*/
unsigned int tlb_get_frame(TLB* tlb, unsigned int page) {
    for (int i = 0; i < TLB_SIZE; i++){
        TLB_Entry* node = tlb->entries[i];
        if (node->page == page) {
            node->timestamp = 0; // Resetear timestamp ya que se acaba de usar
            return node->frame;
        }
    }
    return TLB_MISS; // No se encontro en la TLB
}

// Setea la entrada LRU
void tlb_update_lru(TLB* tlb) {
    for (int i = 0; i < TLB_SIZE; i++){
        TLB_Entry* node = tlb->entries[i];
        if (tlb->max_timestamp < node->timestamp) {
            tlb->max_timestamp = node->timestamp;
            tlb->lru = node;
        }
    }
}

// Mete el frame a la TLB segun LRU
void tlb_set(TLB* tlb, unsigned int page, unsigned int frame) {
    if (!tlb->is_full) { // Todavia se esta llenando la TLB
        for (int i = 0; i < TLB_SIZE; i++){
            TLB_Entry* node = tlb->entries[i];
            if (0 == node->page) {
                node->page = page;
                node->frame = frame;
                node->timestamp = 0;
                if (i == TLB_SIZE - 1) tlb->is_full = true; // Se lleno la TLB
                return;
            }
        }
    }

    // If full, run LRU

    tlb_update_lru(tlb); // Set max_timestamp y lru
    TLB_Entry* node = tlb->lru;
    node->page = page;
    node->frame = frame;
    node->timestamp = 0;
    tlb_update_lru(tlb); // Reordenar
    return;
}

/** Libera todos los recursos asociados a la TLB */
void tlb_destroy(TLB* tlb) {
    for (int i = 0; i < TLB_SIZE; i++){
        TLB_Entry* node = tlb->entries[i];
        free(node);
    }
    // free(tlb);
}

// Setea la entrada LRU
void mem_update_lru(Memory* mem) {
    for (int i = 0; i < RAM_SIZE; i++){
        RAM_Entry* node = mem->frames[i];
        if (mem->max_timestamp < node->timestamp) {
            mem->max_timestamp = node->timestamp;
            mem->lru = node;
        }
    }
}

PageTable* page_table_init(unsigned int size) {
    PageTable* pt; // = malloc(sizeof(TLB));
    pt->entries = malloc(size * sizeof(PageTableEntry));
    // Inicializar entries
    for (int i = 0; i < size; i++) pt->entries[0] = tlb_entry_init();

    return pt;
}


void swap(PageTable* pt, Memory* mem, char* data, unsigned int address) {
    RAM_Entry* lru = mem->lru;

    mem_update_lru(mem);
    obsoletize_page(pt, lru->address); // Indicar que ya no apunta al dato que apuntaba antes
    lru->data = data;
    lru->address = address;
    lru->timestamp = 0;
    mem_update_lru(mem);
    return;
}
