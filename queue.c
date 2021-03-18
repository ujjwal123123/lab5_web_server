#include "request.h"
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Implement circular queue
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t conditional_variable = PTHREAD_COND_INITIALIZER;

#define SIZE 100

struct queue_t {
    struct request **array; // array of all the items
    int start;              // index from which item should be removed
    int end;                // index at which item should be added
};

struct queue_t *new_queue() {
    struct queue_t *queue = (struct queue_t *)malloc(sizeof(struct queue_t));
    queue->start = 0; // index from which an item should be removed
    queue->end = 0;   // index at which new item should be added
    queue->array = (struct request **)malloc(sizeof(int) * SIZE);

    return queue;
}

struct request *remove_queue(struct queue_t *queue) {
    assert(queue != NULL);
    assert(queue->array != NULL);

    printf("[Thread] waiting\n");

    pthread_mutex_lock(&mutex);
    printf("[Thread] Lock acquired\n");
    pthread_cond_wait(&conditional_variable, &mutex);

    printf("[Queue] before removing, start: %d, end : %d\n", queue->start,
           queue->end);
    assert(queue->start != queue->end);

    struct request *ret_val = queue->array[queue->start];
    assert(ret_val);
    assert(ret_val->filename);
    queue->start = (queue->start + 1) % SIZE;

    printf("[Queue] Removed from the queue %s (TID %lu)\n", ret_val->filename,
           pthread_self());
    pthread_mutex_unlock(&mutex);
    return ret_val;
}

bool is_empty(struct queue_t *queue) { return queue->start == queue->end; }

bool is_full(struct queue_t *queue) { return queue->start == queue->end; }

void insert_queue(struct queue_t *queue, struct request *item_to_be_inserted) {
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&conditional_variable);

    assert(item_to_be_inserted->filename);

    printf("[Queue] inserting into queue: %s (TID: %lu)\n",
           item_to_be_inserted->filename, pthread_self());

    queue->array[queue->end] = item_to_be_inserted;
    queue->end = (queue->end + 1) % SIZE;
    printf("[Queue] after inserting, start: %d, end : %d\n", queue->start,
           queue->end);
    pthread_mutex_unlock(&mutex);
}