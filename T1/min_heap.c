//
// Created by Sebastian Guerra Bugedo on 22/03/2018.
//

#include <stdlib.h>
#include <stdio.h>
#include "heaps.h"


/*
 * heap_init:
 * Creación de un max heap inicialmente igual a un arreglo cualquiera
 */
Heap* heap_init(int *array, int length){
    Heap* my_heap = malloc(sizeof(Heap));
    my_heap->size = length; // largo del array completo (no necesariamente "heapeado")
    my_heap->count = 0;     // conteo de elementos correctamente "heapeados" al comienzo del arreglo
    my_heap->array = array; // puntero al arreglo donde trabajaremos
    return my_heap;
}

/*
 * Los tres métodos siguientes entregan la posición del padre, hijo izquierdo y derecho
 * dada una posición en un arreglo indexado desde 0.
 */

int father_pos(int pos){
    return (pos + 1)/2 - 1;
}

int left_child(int pos){
    return (pos + 1) * 2 - 1;
}

int right_child(int pos){
    return (pos + 1) * 2;
}

/*
 * sift_up:
 * Sube un elemento hasta ubicarlo correctamente en el max heap.
 * Este método se llama luego de insertar un nuevo elemento al final del arreglo "heapeado"
 */
void sift_up(Heap* heap, int current){
    if (father_pos(current) >= 0){
        int father = father_pos(current);
        int* arr = heap->array;
        if (arr[father] < arr[current]){    // verificamos propiedad del max heap: padre > hijo
            int aux = arr[father];          // entramos si están cambiados
            arr[father] = arr[current];
            arr[current] = aux;

            sift_up(heap, father);          // propagamos hacia arriba
        };
    }
}

/*
 * sift_down:
 * Baja un elemento hasta ubicarlo correctamente en el max heap.
 * Este método se llama luego de reemplazar la raíz del heap con
 * el último elemento del arreglo "heapeado"
 */
void sift_down(Heap *heap, int current){
    int child;
    int *arr = heap->array;
    if (right_child(current) < heap->count){       // si hay 2 hijos, nos interesa analizar el hijo más grande
        if (arr[right_child(current)] > arr[left_child(current)]){
            child = right_child(current);
        } else {
            child = left_child(current);
        }
    } else if (left_child(current) < heap->count){ // si solo hay 1 hijo, lo tomamos para analizar
        child = left_child(current);
    } else {                                        // si no hay, no entraremos al intercambio
        child = 0;
    }
    if (child > 0){
        if (arr[child] > arr[current]){             // verificamos propiedad del heap con el hijo analizado
            int aux = arr[child];                   // intercambiamos si no se cumple
            arr[child] = arr[current];
            arr[current] = aux;

            sift_down(heap, child);                 // propagamos hacia abajo
        }
    }
}

/*
 * insert:
 * Agrega un elemento a la parte "heapeada" del arreglo que guarda el heap.
 * Como trabajamos con un mismo arreglo, sin agregar más elementos que los del arreglo mismo,
 * no nos preocupamos de que se nos acabe el espacio del arreglo (definido por heap->size).
 */
void insert(Heap* heap, int new_int){
    int blank = heap->count;        // posicion del 1er espacio después de la zona "heapeada" del arreglo
    heap->array[blank] = new_int;   // guardamos ahí el elemento, y queda como nodo hoja
    heap->count++;                  // ahora decimos que la zona "heapeada" creció
    sift_up(heap, blank);           // posicionamos bien el elemento agregado, propagando hacia arriba
}

/*
 * extract:
 * Retorna la raíz del heap y la elimina, reemplazándola con el último elemento de
 * la zona "heapeada" del arreglo. Al extraer, se reduce en 1 el tamano de la zona "heapeada"
 * pero el tamaño del arreglo completo se mantiene.
 */
int extract(Heap* heap){
    int last = heap->count-1;       // posicion del último elemento "heapeado"
    int* arr = heap->array;
    int root = arr[0];              // raíz original

    heap->count--;                  // ahora hay 1 elemento menos en la zona "heapeada"
    if (heap->count){               // si quedan elementos en la zona "heapeada"
        arr[0] = arr[last];         // ponemos como raíz al último
        sift_down(heap, 0);         // lo posicionamos correctamente y propagamos hacia abajo
    }
    return root;                    // IMPORTANTE... tenemos que retorna!!!
}

/*
 * build_heap_by_insertion:
 * Dado un arreglo, construye un heap agregando de a uno los
 * elementos del arreglo a la zona "heapeada".
 */
Heap* build_heap_by_insertion(int* array, int length){
    Heap* heap = heap_init(array, length);      // creamos el heap, inicialmente con zona "heapeada" nula
    show_heap(heap);
    for (int i = 0; i < length; ++i) {          // insertamos cada elemento de la zona "no heapeada"
        insert(heap, heap->array[i]);
        show_heap(heap);
    }
    return heap;                                // retornamos el heap completamente "heapeado"
}

/*
 * build_heap_by_heapify:
 * Dado un arreglo, construye un heap moviendo a su posición
 * correcta los elementos de la 1ra mitad del arreglo. Esto tambien
 * reubica los elementos de la 2da mitad con ayuda del sift_down
 */
Heap* build_heap_by_heapify(int* array, int length){
    Heap* heap = heap_init(array, length);      // creamos el heap, inicialmente con zona "heapeada" nula
    heap->count = heap->size;                   // decimos que está completamente "heapeado"
    for (int j = length / 2 - 1; j >= 0; --j) {
        sift_down(heap, j);                     // bajamos los elementos de la primera mitad
        show_heap(heap);
    }
    return heap;
}

/*
 * heap_sort:
 * Recibe un heap (ya "heapeado") y extrae sus elementos poniéndolos
 * al final del arreglo que lo define. Dado que el heap usado es un max heap,
 * debemos ir agregando los elementos al final de derecha a izquierda.
 */
void heap_sort(Heap* heap){
    int my_max;
    for (int i = 0; i < heap->size; ++i) {      // extraemos todos los elementos... por ello usamos heap->size
        my_max = extract(heap);                 // extraemos la raíz actual (el más grande de todos los "heapeados")
                                                // en este paso, la zona "heapeada" se redujo en uno, y quedó un espacio
        heap->array[heap->count] = my_max;      // ponemos el elemento en la primera posición después del heap
        show_heap_and_ordered(heap);
    }
}

/*
 * verify_max_heap:
 * Retorna 1 si el heap cumple la propiedad padre > hijos (o sea, es realmente un heap).
 * Solo revisa si la prop se cumple para la zona "heapeada" (hasta heap->count).
 */
int verify_max_heap(Heap* heap){
    int *arr = heap->array;
    for (int i = 1; i < heap->count / 2; ++i) {
        if (right_child(i) < heap->count){
            if (arr[i] < arr[right_child(i)] || arr[i] < arr[left_child(i)]){
                return 0;
            }
        } else if (left_child(i) < heap->count){
            if (arr[i] < arr[left_child(i)]){
                return 0;
            }
        }
    }
    return 1;
}

void valid_heap(Heap* heap){
    printf("\n");
    if (verify_max_heap(heap)){
        printf("It's a heap!\n");
    } else {
        printf("It's not a heap\n");
    }
}


/*
 * show_heap:
 * Muestra el heap completo en forma de arreglo. Los elementos
 * se organizan por nivel: 1, 2, 4, 8, ..., 2^n
 */
void show_heap(Heap* heap){
    printf("======= Printing Heap =======\n");
    if (!heap->count){
        printf("|");
    }
    for (int i = 0; i < heap->count; ++i) {
        printf("| %d ", heap->array[i]);
    }
    printf("|\n");
}

/*
 * show_heap_and_ordered:
 * Muestra la parte "heapeada" y la parte "ordenada" al ser llamado en heap_sort.
 * Ambas partes van separadas con "||".
 */
void show_heap_and_ordered(Heap* heap){
    printf("======= Printing Heap =======\n");
    for (int i = 0; i < heap->size; ++i) {
        printf("| %d ", heap->array[i]);
        if (heap->count - 1 == i){
            printf("|");
        }
    }
    printf("|\n");
}

/*
 * show_array:
 * Muestra el arreglo inicial
 */
void show_array(int* array, int length){
    printf("======= Printing Array =======\n");
    if (!length){
        printf("|");
        return;
    }
    for (int i = 0; i < length; ++i) {
        printf("| %d ", array[i]);
    }
    printf("|\n");
}

/*
 * destroy_heap:
 * Libera los recursos del heap
 */
void destroy_heap(Heap* heap){
    free(heap->array);
    free(heap);
}