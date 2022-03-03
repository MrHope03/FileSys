#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct node{
	char name[10];
	bool dir;
	struct node *adj;
	struct node *next;
}node_t;

node_t *createNode(char *val, bool dir);
void addNode(node_t *parent,node_t *child);
void removeNode(node_t *curr,node_t *child);

#endif
