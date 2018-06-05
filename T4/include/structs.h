#ifndef STRUCTS_H
#define STRUCTS_H

// ArrayList
typedef struct {
    int count;
    int size;
    void* pointer;
} ArrayList;

/** Crea una arraylist inicialmente vacia y retorna su puntero */
ArrayList* arraylist_init();

/** Inserta un elemento al final de la arraylist */
void arraylist_append(ArrayList* list, void* element);

/** Inserta el elemento dado en la posicion indicada */
void arraylist_insert(ArrayList* list, void* element, int position);

/** Elimina el elemento de la posicion indicada y lo retorna */
void* arraylist_delete(ArrayList* list, int position);

/** Retorna el valor del elemento en la posicion dada */
void* arraylist_get(ArrayList* list, int position);

/** Concatena la segunda arraylist a la primera arraylist */
void arraylist_concatenate(ArrayList* list1, ArrayList* list2);

/** Libera todos los recursos asociados a la lista */
void arraylist_destroy(ArrayList* list);


#endif