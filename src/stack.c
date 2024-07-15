#include "../include/stack.h"
#include "../include/node.h"

#define INITIAL_CAPACITY 10

Stack *sysStack;

// Implementation
Stack *createStack() {
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  stack->capacity = INITIAL_CAPACITY;
  stack->top = -1;
  stack->items = (sysNode **)malloc(sizeof(sysNode *) * stack->capacity);
  return stack;
}

void push(Stack *stack, sysNode *item) {
  if (stack->top == stack->capacity - 1) {
    // Stack is full, resize it
    stack->capacity *= 2;
    stack->items = (sysNode **)realloc(stack->items, sizeof(sysNode *) * stack->capacity);
  }
  stack->items[++stack->top] = item;
}

sysNode *pop(Stack *stack) {
  if (isEmpty(stack)) {
    return NULL;
  }
  return stack->items[stack->top--];
}

sysNode *peek(Stack *stack) {
  if (isEmpty(stack)) {
    return NULL;
  }
  return stack->items[stack->top];
}

int isEmpty(Stack *stack) {
  return stack->top == -1;
}

void destroyStack(Stack *stack) {
  free(stack->items);
  free(stack);
}
