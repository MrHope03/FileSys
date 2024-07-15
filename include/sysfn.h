#ifndef SYSFN_H
#define SYSFN_H

#include <errno.h>
#include <sys/stat.h>

#include "editor.h"
#include "node.h"
#include "stack.h"
#include "utils.h"

sysNode *dirMacro(const char *str);
sysNode *findRootfromChained(char *str);
void listDir(char *nodeName, bool curr);
void effectSysStack(sysNode *root);
void changeDir(char *dir);
char *returnFileName(char *nodeName);
void makeDirectory(char *nodeName);
void createFile(char *nodeName);
void showFile(char *nodeName);
void openFile(char *nodeName);
void deleteFile(char *nodeName);
void deleteFolder(char *nodeName);
void initialiseFileSystem(char *projectname);
void shutdownFileSystem(char *projectname);
char *pwd(char *delim);
void prompt();

#endif
