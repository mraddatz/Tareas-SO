// Esta linea sirve para que el código de este archivo solo se importe una vez
#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include "include/structs.h"

//////////////////////////////////////////////////////////////////////////
//                             Funciones                                //
//////////////////////////////////////////////////////////////////////////

/** Crea una arraylist inicialmente vacia y retorna su puntero */
ArrayList* arraylist_init();

/** Inserta un elemento al final de la arraylist */
void arraylist_append(ArrayList* list, Process* element);

/** Inserta el elemento dado en la posicion indicada */
void arraylist_insert(ArrayList* list, Process* element, int position);

/** Elimina el elemento de la posicion indicada y lo retorna */
Process* arraylist_delete(ArrayList* list, int position);

/** Retorna el valor del elemento en la posicion dada */
Process* arraylist_get(ArrayList* list, int position);

/** Concatena la segunda arraylist a la primera arraylist */
void arraylist_concatenate(ArrayList* list1, ArrayList* list2);

/** Libera todos los recursos asociados a la lista */
void arraylist_destroy(ArrayList* list);

#endif
