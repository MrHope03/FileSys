#ifndef STACK_H
#define STACK_H

#include "../include/node.h"

// Stack def
typedef struct {
  sysNode **items;
  int capacity;
  int top;
} Stack;

// Function prototypes
Stack *createStack();
void push(Stack *stack, sysNode *item);
sysNode *pop(Stack *stack);
sysNode *peek(Stack *stack);
int isEmpty(Stack *stack);
void destroyStack(Stack *stack);

#endif
