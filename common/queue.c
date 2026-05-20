#include "queue.h"
#include <stdio.h>

void queue_init(Queue *q) {
    q->front = q->rear = q->size = 0;
}

int queue_empty(Queue *q) {
    return q->size == 0;
}

void enqueue(Queue *q, PCB *p) {
    q->data[q->rear] = p;
    q->rear = (q->rear + 1) % MAX_PROCS;
    q->size++;
}

PCB *dequeue(Queue *q) {
    if (queue_empty(q)) return NULL;
    PCB *p = q->data[q->front];
    q->front = (q->front + 1) % MAX_PROCS;
    q->size--;
    return p;
}

void queue_print(Queue *q) {
    if (queue_empty(q)) { printf("Empty\n"); return; }
    for (int i = 0, idx = q->front; i < q->size; i++) {
        printf("%s ", q->data[idx]->name);
        idx = (idx + 1) % MAX_PROCS;
    }
    printf("\n");
}
