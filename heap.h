#define DEFAULT_HEAP_LENGTH 100

typedef struct heap_t {
    struct request **array;
    int length;   // gives the (maximum) number of elements in the array
    int heapSize; // how many elements in the heap are stored within
                  // the array
} HEAP;


void maxHeapify(HEAP *heap, int parent_index);
struct request *heapMaximum(HEAP *heap);
struct request *heapExtractMaximum(HEAP *heap);
void maxHeapInsert(HEAP *heap, struct request *key);
