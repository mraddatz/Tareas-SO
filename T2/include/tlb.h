#ifndef TLB_H
#define TLB_H

#include <stdbool.h>

// TLB_Entry
typedef struct tlb_node {
    unsigned int page;
    unsigned int frame;
    unsigned int timestamp;
    struct tlb_node* next;
} TLB_Entry;

// TLB
typedef struct {
    TLB_Entry* root;
    TLB_Entry* last;
    bool is_full;
    int size;
    unsigned int clock;
    unsigned int lru;
    unsigned int max_timestamp;
} TLB;

/** Crea una TLB inicialmente vacia y retorna el puntero */
TLB* tlb_init();

/** Inserta un elemento al final de la TLB */
void tlb_append(TLB* tlb, unsigned int page, unsigned int frame);

/** Retorna el frame del elemento en la posicion dada */
unsigned int tlb_get_frame(TLB* tlb, unsigned int page);

// Mete el frame a la TLB segun LRU
void tlb_update_lru(TLB* tlb);

// Mete el frame a la TLB segun LRU
void tlb_set(TLB* tlb, unsigned int page, unsigned int frame);

/** Libera todos los recursos asociados a la TLB */
void tlb_destroy(TLB* tlb);

#endif