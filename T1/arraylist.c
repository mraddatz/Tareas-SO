// Importo el archivo arraylist.h
#include "include/arraylist.h"
// Libreria estandar de C
#include <stdlib.h>

#define INITSIZE 8

//////////////////////////////////////////////////////////////////////////
//                             Funciones                                //
//////////////////////////////////////////////////////////////////////////

// TODO: Debes completar las siguientes funciones
// Puedes crear otras funciones aca para el
// funcionamiento interno del arreglo dinamico

/** Crea un nuevo array y retorna su puntero */
ArrayList* arraylist_realloc(ArrayList* list)
{
    Process* new_array;
    new_array = malloc(2 * list->size * sizeof(Process));
    for (int i = 0; i < list->size; i++){
        new_array[i] = list->pointer[i];
    }
    list->size *= 2;
    free(list->pointer);
    list->pointer = new_array;
    return list;
}

/** Crea una arraylist inicialmente vacia y retorna su puntero */
ArrayList* arraylist_init()
{
    ArrayList *list = malloc(sizeof(ArrayList));
    list->size = INITSIZE;  /* Tamaño inicial del array */
    list->count = 0;  /* Comienza con 0 elementos */
    list->pointer = malloc(INITSIZE * sizeof(Process));  /* array de 16 int */

    return list;
}

/** Inserta un elemento al final de la arraylist */
void arraylist_append(ArrayList* list, Process* element)
{
    if (list->count == list->size){
        list = arraylist_realloc(list);
    }
    list->pointer[list->count] = *element;
    list->count += 1;
}

/** Inserta el elemento dado en la posicion indicada */
void arraylist_insert(ArrayList* list, Process* element, int position)
{
    // al final == list->count
    // cualquier otra hay que mover datos desde pos hast count
    if (list->count == list->size){
        list = arraylist_realloc(list);
    }
    for (int i = list->count; i >= 0; i--){
        if (i == position){
            list->pointer[i] = *element;  /* lo agrego si estoy en posicion */
            break;
        }
        else {
            list->pointer[i] = list->pointer[i-1];  /* reordeno array eoc */
        }
    }
    list->count += 1;
}

/** Elimina el elemento de la posicion indicada y lo retorna */
Process* arraylist_delete(ArrayList* list, int position)
{
    Process* aux;
    aux = &(list->pointer[position]);
    for (int i = position; i < list->count - 1; i++){
        list->pointer[i] = list->pointer[i+1];  /* reordeno array */
    }
    list->count -= 1;

    return aux;
}

/** Retorna el valor del elemento en la posicion dada */
Process* arraylist_get(ArrayList* list, int position)
{
    return &(list->pointer[position]);
}

/** Concatena la segunda arraylist a la primera arraylist */
void arraylist_concatenate(ArrayList* list1, ArrayList* list2)
{
    while (list1->size < list1->count + list2->count){
        arraylist_realloc(list1);
    }
    for (int i = 0; i < list2->count; i++){
        list1->pointer[list1->count] = list2->pointer[i];
        list1->count += 1;
    }
    arraylist_destroy(list2);
}

/** Libera todos los recursos asociados a la lista */
void arraylist_destroy(ArrayList* list)
{
    free(list->pointer);
    free(list);
}
