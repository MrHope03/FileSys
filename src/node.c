#include "../include/node.h"

sysNode *createNode(char *val, bool dir) {
  sysNode *newNode = (sysNode *)malloc(sizeof(sysNode));
  if (newNode == NULL) {
    perror("Failed to create System Node");
    exit(1);
  }
  newNode->name = strdup(val);
  newNode->dir = dir;
  newNode->nextSibling = newNode->firstChild = NULL;
  return newNode;
}

void addNode(sysNode *parent, sysNode *child) {
  if (parent->firstChild == NULL) {
    parent->firstChild = child;
  } else {
    sysNode *p = parent->firstChild;
    while (p->nextSibling != NULL)
      p = p->nextSibling;
    p->nextSibling = child;
  }
}

void removeNode(sysNode *parent, char *nodeName) {
  sysNode *p = parent->firstChild;
  if (strcmp(p->name, nodeName) == 0) {
    parent->firstChild = p->nextSibling;
    free(p);
    return;
  }
  while (strcmp(p->nextSibling->name, nodeName) != 0) {
    p = p->nextSibling;
  }
  sysNode *temp = p->nextSibling;
  p->nextSibling = p->nextSibling->nextSibling;
  free(temp);
}

void viewSystemOneLevel(sysNode *root) {
  sysNode *p = root->firstChild;
  while (p != NULL) {
    if (p->dir)
      printf("%s/ ", p->name);
    else
      printf("%s ", p->name);
    p = p->nextSibling;
  }
  printf("\n");
}

void viewSystemTree(sysNode *root, int depth) {
  if (root == NULL) return;

  printIndent(depth);
  if (depth > 0) printf("|_ ");
  if (root->dir)
    printf("%s/\n", root->name);
  else
    printf("%s\n", root->name);
  sysNode *child = root->firstChild;
  while (child != NULL) {
    viewSystemTree(child, depth + 1);
    child = child->nextSibling;
  }
}

sysNode *findSystemNode(sysNode *root, char *nodeName) {
  sysNode *p = root->firstChild;
  while (p != NULL) {
    if (strcmp(p->name, nodeName) == 0) return p;
    p = p->nextSibling;
  }
  return NULL;
}

sysNode *findSystemNodeChained(sysNode *root, const char *str, bool hasRoot) {
  char *copy = strdup(str);
  sysNode *curr = NULL;
  char *token = strtok(copy, "/");
  if (hasRoot)
    token = strtok(NULL, "/");
  while (token != NULL) {
    root = findSystemNode(root, token);
    if (root == NULL) {
      perror("Failed to find path");
      return NULL;
    }
    token = strtok(NULL, "/");
  }
  return root;
}

void saveNodeToBinary(sysNode *root, FILE *file) {
  if (root == NULL) {
    int nullMarker = -1;
    fwrite(&nullMarker, sizeof(int), 1, file);
    return;
  }

  int nameLength = strlen(root->name);
  fwrite(&nameLength, sizeof(int), 1, file);

  fwrite(root->name, sizeof(char), nameLength, file);

  fwrite(&(root->dir), sizeof(bool), 1, file);

  saveNodeToBinary(root->firstChild, file);
  saveNodeToBinary(root->nextSibling, file);
}

sysNode *readNodeFromBinary(FILE *file) {
  int nameLength;
  if (fread(&nameLength, sizeof(int), 1, file) != 1) {
    return NULL;
  }

  if (nameLength == -1) {
    return NULL;
  }

  sysNode *node = malloc(sizeof(sysNode));
  if (node == NULL) {
    perror("Failed to create sys node");
    return NULL;
  }

  node->name = malloc(nameLength + 1);
  if (node->name == NULL) {
    perror("Failed to create memory of name");
    free(node);
    return NULL;
  }
  fread(node->name, sizeof(char), nameLength, file);
  node->name[nameLength] = '\0';

  fread(&(node->dir), sizeof(bool), 1, file);

  node->firstChild = readNodeFromBinary(file);
  node->nextSibling = readNodeFromBinary(file);

  return node;
}
