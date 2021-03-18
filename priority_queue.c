// max priority queues using heaps

// #include "heap.h"

// typedef HEAP PRIORITY_QUEUE;
#include "priority_queue.h"
#include <stdio.h>
#include <stdlib.h>

PRIORITY_QUEUE *init_priority_queue() {
    HEAP *newHeap = (HEAP *)malloc(sizeof(HEAP));
    newHeap->array =
        (struct request **)malloc(sizeof(int) * DEFAULT_HEAP_LENGTH);
    newHeap->length = DEFAULT_HEAP_LENGTH;
    newHeap->heapSize = 0;

    printf("[Priority Queue] Intialized priority queue\n");
    return newHeap;
}

struct request *maximum(PRIORITY_QUEUE *priority_queue) {
    return heapMaximum(priority_queue);
}

struct request *extract_maximum(PRIORITY_QUEUE *priority_queue) {
    printf("[Priority queue] Removing the maximum from the priority queue\n");
    return heapExtractMaximum(priority_queue);
}

// inline void increaseKey(PRIORITY_QUEUE *priority_queue, int index, int key) {
//     return heapIncreaseKey(priority_queue, index, key);
// }

void insert_priority_queue(PRIORITY_QUEUE *priority_queue,
                           struct request *key) {
    printf("[Priority queue] Inserting into the priority queue\n");
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