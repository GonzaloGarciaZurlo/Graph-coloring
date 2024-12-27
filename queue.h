#ifndef __QUEUE_H
#define __QUEUE_H
#include <stdbool.h>
#include "EstructuraGrafo.h"

struct queue {
  u32 max_size;
  u32* elems;
  u32 head;
  u32 tail;
  u32 size;
};

void initqueue(struct queue*, u32);
void enqueue(struct queue*, u32);
u32 dequeue(struct queue*);
bool emptyqcheck(struct queue*);
void destroyqueue(struct queue*);

#endif