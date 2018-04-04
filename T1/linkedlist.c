// importo el archivo linkedlist.h
#include "linkedlist.h"
// Libreria estandar de C
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////
//                             Funciones                                //
//////////////////////////////////////////////////////////////////////////

// TODO: debes completar las funciones que estan a continuacion
// Puedes crear otras funciones aca para el
// funcionamiento interno del arreglo dinamico

/** Crea una lista inicialmente vacia y retorna el puntero */
LinkedList* linkedlist_init()
{
    LinkedList *list = malloc(sizeof(LinkedList));
    list->size = 0;
    list->root = NULL;
    list->last = NULL;
    return list;
}


/** Inserta un elemento al final de la lista */
void linkedlist_append(LinkedList* list, int element)
{
    LinkedListNode *new_node = malloc(sizeof(LinkedListNode));
    if (list->size == 0){
        list->root = new_node;
    }
    else {
        list->last->next = new_node;
    }
    new_node->data = element;
    new_node->next = NULL;
    list->last = new_node;
    list->size += 1;
}

/** Inserta el elemento dado en la posicion indicada */
void linkedlist_insert(LinkedList* list, int element, int position)
{
    LinkedListNode *aux, *new_node = malloc(sizeof(LinkedListNode));
    if (position == 0){
        new_node->next = list->root;
        list->root = new_node;
    }
    else if (position == list->size){
        list->last->next = new_node;
        list->last = new_node;
        new_node->next = NULL;
    }
    else {
        aux = list->root;
        for (int i = 0; i < position - 1; i ++){
            aux = aux->next;
        }
        new_node->next = aux->next;
        aux->next = new_node;
    }

    new_node->data = element;
    list->size += 1;
}

/** Elimina el elemento de la posicion indicada y lo retorna */
int linkedlist_delete(LinkedList* list, int position)
{
    int num;
    LinkedListNode *aux, *aux2;
    if (position == 0){
        aux = list->root;
        list->root = aux->next;
        num = aux->data;
        free(aux);
        list->size -= 1;
        return num;
    }
    else {
        aux = list->root;
        for (int i = 0; i < position - 1; i++){
            aux = aux->next;
        }
        num = aux->next->data;
        if (position == list->size - 1){
            free(aux->next);
            aux->next = NULL;
            list->last = aux;
        }
        else {
            aux2 = aux->next;
            aux->next = aux->next->next;
            free(aux2);
        }
        list->size -= 1;
        return num;
    }
}

/** Retorna el valor del elemento en la posicion dada */
int linkedlist_get(LinkedList* list, int position)
{
    LinkedListNode *node = list->root;
    for (int i = 0; i < position; i++){
        node = node->next;
    }
    return node->data;
}

/** Concatena a la lista una segunda lista */
void linkedlist_concatenate(LinkedList* list, LinkedList* list2)
{
    list->last->next = list2->root;
    list->last = list2->last;
    list->size += list2->size;

    free(list2);
}

/** Libera todos los recursos asociados a la lista */
void linkedlist_destroy(LinkedList* list)
{
    LinkedListNode *next_node, *node = list->root;
    for (int i = 0; i < list->size; i++){
        next_node = node->next;
        free(node);
        node = next_node;
    }
    free(list);
}
