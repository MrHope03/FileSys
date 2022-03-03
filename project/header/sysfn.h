#ifndef SYSFN_H
#define SYSFN_H

#include "../header/node.h"
#include "../header/stack.h"

void strip(char *str,char delim);
void displayRoot(node_t *root);
node_t *findDir(node_t *curr,char *fileName);
void listDir(node_t *root);
node_t *changeDir(node_t *curr,char *dir);
node_t *returnDir(node_t *curr);
void readFile(node_t *child);
void writeFile(node_t *child);
void deleteFile(node_t *child);
bool startRoot(node_t *root, FILE *file);
node_t *initialiseFileSystem();
bool closeRoot(node_t *root,FILE *file);
void shutdownFileSystem(node_t *root);
void pwd(node_t *curr);
void prompt(node_t *curr);

#endif
