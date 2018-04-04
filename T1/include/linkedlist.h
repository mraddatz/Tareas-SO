// Esta linea sirve para que el codigo de este archivo solo se importe una vez
#ifndef LINKEDLIST_H
#define LINKEDLIST_H


/** Estructura de un node de lista ligada */
struct linked_list_node
{
    int data;
    struct linked_list_node *next;
};

typedef struct linked_list_node LinkedListNode;

/** Estructura de una lista ligada */
struct linked_list
{
    LinkedListNode *root;
    LinkedListNode *last;
    int size;
};

// Aqui le estoy poniendo un nombre mas simple a la lista para no tener que
// referirme a la lista como struct linked_list
/** Estructura de una lista ligada */
typedef struct linked_list LinkedList;


//////////////////////////////////////////////////////////////////////////
//                             Funciones                                //
//////////////////////////////////////////////////////////////////////////

//OJO: No se debe modificar nada de esto

/** Crea una lista inicialmente vacia y retorna el puntero */
LinkedList* linkedlist_init();

/** Inserta un elemento al final de la lista */
void linkedlist_append(LinkedList* list, int element);

/** Inserta el elemento dado en la posicion indicada */
void linkedlist_insert(LinkedList* list, int element, int position);

/** Elimina el elemento de la posicion indicada y lo retorna */
int linkedlist_delete(LinkedList* list, int position);

/** Retorna el valor del elemento en la posicion dada */
int linkedlist_get(LinkedList* list, int position);

/** Concatena la segunda lista a la primera lista */
void linkedlist_concatenate(LinkedList* list1, LinkedList* list2);

/** Libera todos los recursos asociados a la lista */
void linkedlist_destroy(LinkedList* list);

#endif
