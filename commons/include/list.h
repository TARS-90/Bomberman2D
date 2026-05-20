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

List *create_list();
void delete_list_deep(List *l);
void delete_list_shallow(List *l);
void insert(List*, void*);
void *get_at(List*, int);
void remove_at(List*, int);

#endif 
