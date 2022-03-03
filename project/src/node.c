#include "../header/node.h"

node_t *createNode(char *val, bool dir){
	node_t *temp = (node_t *) malloc (sizeof(node_t));
	strcpy(temp->name,val);
	temp->dir = dir;
	temp->adj = temp->next = NULL;
	return temp;
}

void addNode(node_t *parent,node_t *child){
	child->adj = NULL;
	if (parent->next == NULL){	// dir has no files
		parent->next = child;
	}
	else{	// When a dir has files
		node_t *p = parent->next;
		while (p->adj != NULL)
			p = p->adj;
		p->adj = child;
	}
}

void removeNode(node_t *curr,node_t *child){
	node_t *p = curr;
	if (p->next == child){	// first child removal
		p->next = child->adj;
	}
	else{
		p = curr->next;
		while (p->adj != child)
			p = p->adj;
		p->adj = child->adj;
	}
	free(child);
}
