#ifndef STACK_H
#define STACK_H

#include "../header/node.h"

// Stack def
typedef struct list{
	node_t *dir;
	struct list *next;
}list_t;

typedef struct{
	list_t *top;
}Stack;

list_t *createListNode(node_t *dir);
void push(node_t *dir);
node_t *pop();

#endif
