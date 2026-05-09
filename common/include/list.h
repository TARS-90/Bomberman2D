#ifndef LIST_H
#define LIST_H

#include <stddef.h>

typedef struct Node Node;
typedef struct List List;

struct List {
	Node *head;
	Node *tail;
	size_t size;
};

List *createList();
void deleteList(List *l);
void insert(List*, void*);
void *getAt(List*, int);
void removeAt(List*, int);

#endif 
