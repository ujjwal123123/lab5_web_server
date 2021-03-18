#include "heap.h"
#include "request.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// #define DEFAULT_HEAP_LENGTH 100

void printArray(int *array, int length) {
    for (int i = 0; i < length; i++)
        printf("%d ", array[i]);

    printf("\n");
}


static inline int parent(int index) { return (index - 1) / 2; }

static inline int left(int index) { return 2 * index + 1; }

static inline int right(int index) { return 2 * index + 2; }

static inline void swap(struct request **a, struct request **b) {
    struct request *temp = *a;
    *a = *b;
    *b = temp;
}

// TODO: what is the use of parent here?
void maxHeapify(HEAP *heap, int parent_index) {
    int l = left(parent_index);
    int r = right(parent_index);
    struct request **array = heap->array;

    // identify the largest of parent, l, r and store in largest
    int largest = parent_index;

    if (l < heap->heapSize &&
        array[l]->filesize > array[parent_index]->filesize)
        largest = l;

    if (r < heap->heapSize && array[r]->filesize > array[largest]->filesize)
        largest = r;

    if (largest != parent_index) {
        swap(&array[largest], &array[parent_index]);

        maxHeapify(heap, largest);
    }
}

// HEAP* newHEAP()
// {
//     HEAP* heap = (HEAP*)malloc(sizeof(HEAP));
//     heap->array = (void*)malloc(sizeof(void*) *
//     DEFAULT_HEAP_LENGTH); heap->length = DEFAULT_HEAP_LENGTH;
//     heap->heapSize = 0;
//     return heap;
// }

// returns a heap for a corresponding array,
// Note: there is no newHeap function, use this instead
// HEAP *buildMaxHeap(struct request **array, int length) {
//     HEAP *heap = (HEAP *)malloc(sizeof(HEAP));
//     heap->length = length;
//     heap->heapSize = length;
//     heap->array = array;

//     for (int i = length / 2; i >= 0; i--)
//         maxHeapify(heap, i);

//     return heap;
// }

// following functions are used by priority queues

struct request *heapMaximum(HEAP *heap) {
    assert(heap->heapSize > 0);

    return heap->array[0];
}

struct request *heapExtractMaximum(HEAP *heap) {
    assert(heap->heapSize > 0);

    struct request **array = heap->array;
    struct request *max = array[0];

    array[0] = array[--(heap->heapSize)];
    maxHeapify(heap, 0);

    return max;
}

// TODO: understand
void heapIncreaseKey(HEAP *heap, int index) {
    struct request **array = heap->array;

    // assert(key > array[index]);
    // array[index] = key;

    while (index > 0 &&
           array[parent(index)]->filesize < array[index]->filesize) {
        swap(&array[index], &array[parent(index)]);
        index = parent(index);
    }
}

void maxHeapInsert(HEAP *heap, struct request *key) {
    assert(heap->heapSize + 1 <= heap->length);

    heap->heapSize += 1;

    heap->array[heap->heapSize - 1] = key;

    heapIncreaseKey(heap, heap->heapSize - 1);
}

void printHeap(HEAP *heap) {
    for (int i = 0; i < heap->heapSize; i++) {
        struct request *req = heap->array[i];
        printf("Name: %s, size: %d\n", req->filename, req->filesize);
    }
    printf("\n");
}

// int main()
// {
//     int arr[] = { 10, 0, 8, 7, 6, 100, 4, 3, 200, 1 };

//     heapSort(arr, 10);

//     printArray(arr, 10);
// }