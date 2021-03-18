#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_HEAP_LENGTH 100

void printArray(int *array, int length) {
    for (int i = 0; i < length; i++)
        printf("%d ", array[i]);

    printf("\n");
}

typedef struct heap_t {
    int *array;
    int length;   // gives the (maximum) number of elements in the array
    int heapSize; // how many elements in the heap are stored within
                  // the array
} HEAP;

static inline int parent(int i) { return (i - 1) / 2; }

static inline int left(int i) { return 2 * i + 1; }

static inline int right(int i) { return 2 * i + 2; }

static inline void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// TODO: what is the use of parent here?
void maxHeapify(HEAP *heap, int parent) {
    int l = left(parent);
    int r = right(parent);
    int *array = heap->array;

    // identify the largest of parent, l, r and store in largest
    int largest = parent;

    if (l < heap->heapSize && array[l] > array[parent])
        largest = l;

    if (r < heap->heapSize && array[r] > array[largest])
        largest = r;

    if (largest != parent) {
        swap(&array[largest], &array[parent]);

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
HEAP *buildMaxHeap(int *array, int length) {
    HEAP *heap = (HEAP *)malloc(sizeof(HEAP));
    heap->length = length;
    heap->heapSize = length;
    heap->array = array;

    for (int i = length / 2; i >= 0; i--)
        maxHeapify(heap, i);

    return heap;
}

void heapSort(int *array, int length) {
    HEAP *heap = buildMaxHeap(array, length);

    for (int i = length - 1; i >= 1; i--) {
        swap(&array[i], &array[0]);
        heap->heapSize--;
        maxHeapify(heap, 0);
    }

    free(heap);
}

// following functions are used by priority queues

int heapMaximum(HEAP *heap) {
    assert(heap->heapSize > 0);

    return heap->array[0];
}

int heapExtractMaximum(HEAP *heap) {
    assert(heap->heapSize > 0);

    int *array = heap->array;
    int max = array[0];

    array[0] = array[--(heap->heapSize)];
    maxHeapify(heap, 0);

    return max;
}

// TODO: understand
void heapIncreaseKey(HEAP *heap, int index, int key) {
    int *array = heap->array;

    assert(key > array[index]);
    array[index] = key;

    while (index > 0 && array[parent(index)] < array[index]) {
        swap(&array[index], &array[parent(index)]);
        index = parent(index);
    }
}

void maxHeapInsert(HEAP *heap, int key) {
    assert(heap->heapSize + 1 <= heap->length);

    heap->heapSize += 1;

    heap->array[heap->heapSize - 1] = INT_MIN;

    heapIncreaseKey(heap, heap->heapSize - 1, key);
}

void printHeap(HEAP *heap) {
    for (int i = 0; i < heap->heapSize; i++)
        printf("%d ", heap->array[i]);

    printf("\n");
}

// int main()
// {
//     int arr[] = { 10, 0, 8, 7, 6, 100, 4, 3, 200, 1 };

//     heapSort(arr, 10);

//     printArray(arr, 10);
// }