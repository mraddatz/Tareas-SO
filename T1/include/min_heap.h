#ifndef MIN_HEAP_H
#define MIN_HEAP_H

struct heap{
    int *array;
    int size;
    int count;
};

typedef struct heap Heap;

Heap* heap_init(int* array, int length);

void sift_up(Heap* heap, int current);

void sift_down(Heap *heap, int current);

void insert(Heap* heap, int new_int);

int extract(Heap* heap);

Heap* build_heap_by_insertion(int* array, int length);

Heap* build_heap_by_heapify(int* array, int length);

void heap_sort(Heap* heap);

int verify_max_heap(Heap* heap);

void valid_heap(Heap* heap);

void show_heap(Heap* heap);

void show_heap_and_ordered(Heap* heap);

void show_array(int* array, int length);

void destroy_heap(Heap* heap);

#endif