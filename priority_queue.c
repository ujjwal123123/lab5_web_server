// max priority queues using heaps

#include "heap.c"

typedef HEAP PRIORITY_QUEUE;

PRIORITY_QUEUE *init_priority_queue() {
    HEAP *newHeap = (HEAP *)malloc(sizeof(HEAP));
    newHeap->array = (int *)malloc(sizeof(int) * DEFAULT_HEAP_LENGTH);
    newHeap->length = DEFAULT_HEAP_LENGTH;
    newHeap->heapSize = 0;

    return newHeap;
}

inline int maximum(PRIORITY_QUEUE *priority_queue) {
    return heapMaximum(priority_queue);
}

inline int extractMaximum(PRIORITY_QUEUE *priority_queue) {
    return heapExtractMaximum(priority_queue);
}

inline void increaseKey(PRIORITY_QUEUE *priority_queue, int index, int key) {
    return heapIncreaseKey(priority_queue, index, key);
}

inline void insert(PRIORITY_QUEUE *priority_queue, int key) {
    return maxHeapInsert(priority_queue, key);
}

// int main()
// {
//     int arr[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

//     // heapSort(arr, 10);

//     PRIORITY_QUEUE* queue = init_priority_queue();
//     queue->array = arr;
//     queue->heapSize = 10;

//     printHeap(queue);

//     printf("maximum: %d\n", maximum(queue));
//     printf("extract maximux: %d\n", extractMaximum(queue));

//     printHeap(queue);

//     increaseKey(queue, 9, 100);

//     printHeap(queue);
// }