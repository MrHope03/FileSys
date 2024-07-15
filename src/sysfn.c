#include "../include/sysfn.h"

extern Stack *sysStack;

// System functions
sysNode *dirMacro(const char *str) {
  if (strcmp(str, ".") == 0)
    return peek(sysStack);
  else if (strcmp(str, "..") == 0)
    return sysStack->items[sysStack->top - 1];
  else if (strcmp(str, "~") == 0)
    return sysStack->items[0];
  return NULL;
}

sysNode *findRootfromChained(char *str) {
  char *copy = strdup(str);
  char *token = strtok(copy, "/");
  return dirMacro(token);
}

void listDir(char *nodeName, bool curr) {
  if (curr) {
    viewSystemOneLevel(peek(sysStack));
    return;
  }
  sysNode *targetNode = dirMacro(nodeName);
  if (targetNode == NULL) {
    sysNode *root = findRootfromChained(nodeName);
    targetNode = findSystemNodeChained(root, nodeName, true);
  }
  viewSystemOneLevel(targetNode);
}

void varSysStackChange(sysNode *root) {
  if (root == NULL) return;
  if (root == sysStack->items[sysStack->top - 1]) {
    pop(sysStack);
  } else if (root == sysStack->items[0]) {
    while (sysStack->top > 0)
      pop(sysStack);
  }
}

void makeDirectory(char *nodeName) {
  sysNode *root = peek(sysStack);
  sysNode *child = findSystemNode(root, nodeName);
  if (child != NULL) {
    fprintf(stderr, "The dir %s already exists", nodeName);
    return;
  }
  sysNode *folder = createNode(nodeName, true);
  addNode(root, folder);
}

void changeDir(char *nodeName) {
  sysNode *child = dirMacro(nodeName);
  if (child != NULL) {
    varSysStackChange(child);
    return;
  }
  sysNode *root = findRootfromChained(nodeName);
  if (root == NULL) {
    root = peek(sysStack);
    child = findSystemNodeChained(root, nodeName, false);
    root = NULL;
  } else {
    child = findSystemNodeChained(root, nodeName, true);
  }
  if (child == NULL || !child->dir) {
    fprintf(stderr, "Couldn't locate the dir %s\r\n", nodeName);
    return;
  }
  varSysStackChange(root);

  char *dup = strdup(nodeName);
  char *token = strtok(dup, "/");
  if (root != NULL)
    token = strtok(NULL, "/");
  sysNode *p = peek(sysStack);
  while (token != NULL) {
    p = findSystemNode(p, token);
    push(sysStack, p);
    token = strtok(NULL, "/");
  }
}

char *returnFileName(char *nodeName) {
  char *path = strdup(pwd("#"));
  char *fileName = (char *)calloc(strlen(nodeName) + strlen(path) + 9, sizeof(char));
  sprintf(fileName, "./files/%s#%s", path, nodeName);
  return fileName;
}

void createFile(char *nodeName) {
  char *fileName = strdup(returnFileName(nodeName));
  FILE *fp = fopen(fileName, "r");
  if (fp != NULL) {
    perror("File already exists");
    return;
  }
  sysNode *root = peek(sysStack);
  addNode(root, createNode(nodeName, false));
  fp = fopen(fileName, "w");
  fclose(fp);
}

void showFile(char *nodeName) {
  sysNode *root = peek(sysStack);
  sysNode *fileNode = findSystemNode(root, nodeName);
  if (fileNode == NULL) {
    perror("File doesn't exists");
    return;
  }
  char *fileName = returnFileName(nodeName);
  FILE *file = fopen(fileName, "r");
  if (file == NULL) {
    perror("file doesn't exists");
  }
  char c;
  c = fgetc(file);
  while (!feof(file)) {
    printf("%c", c);
    c = fgetc(file);
  }
  printf("\r\n");
  fclose(file);
}

void openFile(char *nodeName) {
  sysNode *root = peek(sysStack);
  sysNode *fileNode = findSystemNode(root, nodeName);
  if (fileNode == NULL) {
    perror("File created");
    createFile(nodeName);
  }
  char *fileName = returnFileName(nodeName);

  editor(fileName);
}
void deleteFile(char *nodeName) {
  sysNode *root = peek(sysStack);
  sysNode *fileNode = findSystemNode(root, nodeName);
  if (fileNode == NULL || fileNode->dir) {
    perror("File doesn't exists");
    return;
  }
  char *file = strdup(returnFileName(nodeName));
  removeNode(root, nodeName);
  if (!remove(file)) {
    printf("File %s deleted\r\n", nodeName);
  } else
    fprintf(stderr, "File %s could't be deleted\r\n", nodeName);
}

void deleteFolder(char *nodeName) {
  sysNode *root = peek(sysStack);
  sysNode *folderNode = findSystemNode(root, nodeName);
  if (folderNode == NULL || !folderNode->dir) {
    perror("Folder can't be found");
    return;
  }
  if (folderNode->firstChild != NULL) {
    perror("Folder isn't empty");
    return;
  }
  removeNode(root, nodeName);
}

void initialiseFileSystem(char *projectname) {
  const char *path = "./.note";
  const char *fpath = "./files";
  struct stat st = {0};

  if (stat(path, &st) == -1) {
    if (mkdir(path, 0700) == -1) {
      perror("mkdir");
      exit(1);
    }
    printf("System Directory created\r\n");
  }
  if (stat(fpath, &st) == -1) {
    if (mkdir(fpath, 0700) == -1) {
      perror("mkdir");
      exit(1);
    }
    printf("Files Directory created\r\n");
  }

  char *root_file = (char *)calloc(12 + strlen(projectname), sizeof(char));
  sprintf(root_file, "./.note/%s.bin", projectname);

  FILE *file = fopen(root_file, "rb");
  free(root_file);

  sysNode *root = NULL;
  if (file == NULL) {
    perror("System file doesn't exist. new system file created");
    root = createNode(projectname, true);
  } else {
    root = readNodeFromBinary(file);
    fclose(file);
  }
  sysStack = createStack();
  push(sysStack, root);
}

void shutdownFileSystem(char *projectname) {
  char *root_file = (char *)calloc(10 + strlen(projectname), sizeof(char));
  sprintf(root_file, ".note/%s.bin", projectname);

  FILE *file = fopen(root_file, "wb");
  if (file == NULL) {
    perror("Failed to open system file");
  }

  sysNode *root = dirMacro("~");
  saveNodeToBinary(root, file);
  destroyStack(sysStack);

  free(root_file);
  fclose(file);
}

char *pwd(char *delim) {
  static char path[1024];
  path[0] = '\0';

  if (isEmpty(sysStack)) {
    return path;
  }

  Stack *tempStack = createStack();

  while (!isEmpty(sysStack)) {
    sysNode *node = pop(sysStack);
    push(tempStack, node);
  }

  while (!isEmpty(tempStack)) {
    sysNode *node = pop(tempStack);
    strcat(path, node->name);
    strcat(path, delim);
    push(sysStack, node);
  }

  // Remove the trailing backslash if path is not root
  size_t len = strlen(path);
  if (len > 1) {
    path[len - 1] = '\0';
  }

  destroyStack(tempStack);

  return path;
}

void prompt() {
  printf("%s> ", pwd("/"));
}
