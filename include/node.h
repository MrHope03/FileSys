#ifndef NODE_H
#define NODE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

typedef struct node {
  char *name;
  bool dir;
  struct node *nextSibling;
  struct node *firstChild;
} sysNode;

sysNode *createNode(char *val, bool dir);
void addNode(sysNode *parent, sysNode *child);
void removeNode(sysNode *curr, char *child);
void viewSystemOneLevel(sysNode *root);
void viewSystemTree(sysNode *root, int depth);
sysNode *findSystemNode(sysNode *root, char *nodeName);
sysNode *findSystemNodeChained(sysNode *root, const char *str, bool hasRoot);
void saveNodeToBinary(sysNode *root, FILE *file);
sysNode *readNodeFromBinary(FILE *file);

#endif
