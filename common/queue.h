#ifndef QUEUE_H
#define QUEUE_H

#include "types.h"

typedef struct {
    PCB *data[MAX_PROCS];
    int  front, rear, size;
} Queue;

void  queue_init(Queue *q);
int   queue_empty(Queue *q);
void  enqueue(Queue *q, PCB *p);
PCB  *dequeue(Queue *q);
void  queue_print(Queue *q);   // ps 명령용

#endif
