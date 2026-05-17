#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>
#include <pthread.h>

typedef struct Node Node;
typedef struct Queue Queue;

struct Queue {
	Node *front;
	Node *rear;
	size_t size;
	pthread_mutex_t mutex;
};

Queue *create_queue();
void delete_queue_deep(Queue*);
void delete_queue_shallow(Queue*);
void enqueue(Queue*, void*);
void *dequeue(Queue*);

#endif
