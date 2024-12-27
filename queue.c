#include "queue.h"

void initqueue(struct queue *q,u32 n) {
    q->elems = calloc(n, sizeof(u32));
    q->max_size = n;
    q->head = 0;
    q->tail = -1;  // cuidado,esto es unsiged es igual a MAX_u32
    q->size = 0;
}

void enqueue(struct queue *q, u32 p) {
    if (q->size == q->max_size) {
        fprintf(stderr, "max space in queue\n");
    }
    q->tail = (q->tail + 1) % q->max_size;
    q->elems[q->tail] = p;
    q->size++;
}

u32 dequeue(struct queue *q) {
    u32 p;
    if (q->size == q->max_size) {
        fprintf(stderr, "empty queue\n");
    } else {
        p = q->elems[q->head];
        q->head = (q->head + 1) % q->max_size;
        q->size--;
    }
    return p;
}

bool emptyqcheck(struct queue *q) {
    return 0 == q->size;
}

void destroyqueue(struct queue *q) {
    free(q->elems);
}