#include <stdbool.h>
struct queue_t;

struct queue_t *new_queue();
void *remove_queue(struct queue_t *queue);
bool is_empty(struct queue_t *queue);
bool is_full(struct queue_t *queue);
void insert_queue(struct queue_t *queue, void *item_to_be_inserted);