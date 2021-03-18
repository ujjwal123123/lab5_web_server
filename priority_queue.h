#include "heap.h"

typedef HEAP PRIORITY_QUEUE;

PRIORITY_QUEUE *init_priority_queue();

void insert_priority_queue(PRIORITY_QUEUE *priority_queue, struct request *key);

struct request *extract_maximum(PRIORITY_QUEUE *priority_queue);

struct request *maximum(PRIORITY_QUEUE *priority_queue);