#include "queue.h"
#include <stdlib.h>

struct Node {
	void *value;
	struct Node *next;
	struct Node *prev;
};

Queue *createQueue() {
	Queue *q = malloc(sizeof(Queue));
	q->front = NULL;
	q->rear = NULL;
	q->size = 0;

	return q;
}

// deep deleting
void deleteQueueDeep(Queue *q) {
	if (q == NULL) return;

	while (q->front != NULL) {
		Node *tmp = q->front;
		q->front = tmp->prev;
		free(tmp->value);
		free(tmp);
	}

	free(q);
}

// shallow deleting (values in Nodes are NOT! deleted)
void deleteQueueShallow(Queue *q) {
	if (q == NULL) return;

	while (q->front != NULL) {
		Node *tmp = q->front;
		q->front = tmp->prev;
		free(tmp);
	}

	free(q);
}

void enqueue(Queue *q, void *value) {
	Node *tmp = malloc(sizeof(Node));
	if (!tmp) return;

	tmp->value = value;
	tmp->next = q->rear;
	tmp->prev = NULL;

	if (q->size == 0) {
		q->front = tmp;
		q->rear = tmp;
	} 
	else {
		q->rear->prev = tmp;
		q->rear = tmp;
	}
	q->size++;
}

void *dequeue(Queue *q) {
	if (q->size == 0) return NULL;

	Node *tmp = q->front;
	void *value = tmp->value;
	q->front = tmp->prev;

	if (q->front != NULL) {
		q->front->next = NULL;
	}
	else {
		q->rear = NULL;
	}

	free(tmp);
	q->size--;
	return value;
}




