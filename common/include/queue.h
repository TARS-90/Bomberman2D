#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

typedef struct Node Node;
typedef struct Queue Queue;

struct Queue {
	Node *front;
	Node *rear;
	size_t size;
};

Queue *createQueue();
void deleteQueueDeep(Queue*);
void deleteQueueShallow(Queue*);
void enqueue(Queue*, void*);
void *dequeue(Queue*);

#endif
