#include "include/tlb.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define TLB_SIZE 64
#define TLB_MISS -1

/** Crea una TLB inicialmente vacia y retorna el puntero */
TLB* tlb_init() {
    TLB *tlb = malloc(sizeof(TLB));
    tlb->size = 0;
    tlb->clock = 0;
    tlb->root = NULL;
    tlb->last = NULL;

    for (int i = 0; i < TLB_SIZE; i++) tlb_append(tlb, 0, 0); // Inicia vacia

    return tlb;
}

/** Inserta un elemento al final de la TLB */
void tlb_append(TLB* tlb, unsigned int page, unsigned int frame) {
    TLB_Entry *new_node = malloc(sizeof(TLB_Entry));
    if (tlb->size == 0){
        tlb->root = new_node;
    }
    else {
        tlb->last->next = new_node;
    }
    new_node->page = page;
    new_node->frame = frame;
    new_node->next = NULL;
    tlb->last = new_node;
    tlb->size++;
}

/** Retorna el frame del elemento en la pagina dada
 * Retorna TLB_MISS si no encuentra
*/
unsigned int tlb_get_frame(TLB* tlb, unsigned int page) {
    TLB_Entry* node = tlb->root;
    for (int i = 0; i < TLB_SIZE; i++){
        if (node->page == page) {
            return node->frame;
            node->timestamp = 0;
        }
        node = node->next;
    }
    return NULL;
}

// Setea al LRU
void tlb_update_lru(TLB* tlb) {
    TLB_Entry* node = tlb->root;
    for (int i = 0; i < TLB_SIZE; i++){
        if (tlb->max_timestamp < node->timestamp) {
            tlb->max_timestamp = node->timestamp;
            tlb->lru = node->page;
        }
        node = node->next;
    }
}

// Mete el frame a la TLB segun LRU
void tlb_set(TLB* tlb, unsigned int page, unsigned int frame) {
    TLB_Entry* node = tlb->root;
    if (!tlb->is_full) { // Todavia se esta llenando la TLB
        for (int i = 0; i < TLB_SIZE; i++){
            if (0 == node->page) {
                node->page = page;
                node->frame = frame;
                node->timestamp = 0;
                return;
            }
            node = node->next;
        }
    }

    // If full, run LRU

    tlb_update_lru(tlb); // Set max_timestamp y lru
    for (int i = 0; i < TLB_SIZE; i++){
        if (tlb->lru == node->page) { // node is lru
            node->page = page;
            node->frame = frame;
            node->timestamp = 0;
            return;
        }
        node = node->next;
    }

    tlb_update_lru(tlb); // Reordenar
}

/** Libera todos los recursos asociados a la TLB */
void tlb_destroy(TLB* tlb) {
    TLB_Entry *next_node, *node = tlb->root;
    for (int i = 0; i < tlb->size; i++){
        next_node = node->next;
        free(node);
        node = next_node;
    }
    free(tlb);
}
