#include "../header/node.h"
#include "../header/stack.h"

Stack sys;

list_t *createListNode(node_t *dir){
	list_t *temp = (list_t *) malloc(sizeof(list_t));
	temp->dir = dir;
	temp->next = NULL;
	return temp;
}

// Pushes dir into sys stack
void push(node_t *dir){	
	list_t *temp = createListNode(dir);
	temp->next = sys.top;
	sys.top = temp;
}

// Pops dir from sys stack
node_t *pop(){
	if (sys.top == NULL)
		return NULL;
	list_t *temp = sys.top;
	sys.top = temp->next;
	node_t *retVal = temp->dir;
	free(temp);
	return retVal;
}
