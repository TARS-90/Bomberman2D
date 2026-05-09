#include "list.h"
#include <stdlib.h>

struct Node {
	void *value;
	struct Node *next;
	struct Node *prev;
};

void *createList() {
	List *l = malloc(sizeof(List));
	l->head = NULL;
	l->tail = NULL;
	l->size = 0;

	return l;
}

// shallow deleting (values in Nodes are NOT! deleted)
void deleteList(List *l) {
	if (l == NULL) return;

	while (l->head != NULL) {
		Node *tmp = l->head;
		l->head = tmp->next;
		free(tmp);
	}

	free(l);
}

void insert(List *l, void *value) {
	Node *tmp = malloc(sizeof(Node));
	if (l == NULL) return;
	if (!tmp) return;

	tmp->value = value;
	tmp->next = NULL;
	tmp->prev = l->tail;

	if (l->size != 0) {
		l->tail->next = tmp;
		l->tail = tmp;
	}
	else {
		l->head = tmp;
		l->tail = tmp;
	}

	l->size++;
}

void *getAt(List *l, int index) {
	if (l == NULL || index < 0 || (size_t)index >= l->size) return NULL;

	Node *tmp = l->head;
	for (int i = 0; i < index; i++) {
		tmp = tmp->next;
	}

	return tmp->value;
}

void removeAt(List *l, int index) {
	if (l == NULL || index < 0 || (size_t)index >= l->size) return;
	
	Node *tmp = l->head;
	for (int i = 0; i < index; i++) {
		tmp = tmp->next;
	}

	if (tmp->prev != NULL) {
		tmp->prev->next = tmp->next;
	}
	else {
		l->head = tmp->next;
	}

	if (tmp->next != NULL) {
		tmp->next->prev = tmp->prev;
	}
	else {
		l->tail = tmp->prev;
	}

	free(tmp);
	l->size--;
}

