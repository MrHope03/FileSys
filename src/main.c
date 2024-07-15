#include "../include/editor.h"
#include "../include/sysfn.h"

extern Stack *sysStack;

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: note <project-name>");
    exit(1);
  }
  initialiseFileSystem(argv[1]);
  sysNode *curr = peek(sysStack);

  size_t size = 25;
  char *cmdLine = (char *)malloc(size);

  prompt(curr);
  while (getline(&cmdLine, &size, stdin)) {
    char *cmd = strtok(cmdLine, " ");
    strip(cmd, '\n');
    if (!strcmp(cmd, "pwd")) {
      printf("%s\n", pwd("/"));
    } else if (!strcmp(cmd, "ls")) {
      cmd = strtok(NULL, " ");
      bool flag = false;
      if (cmd == NULL) flag = true;
      listDir(cmd, flag);
    } else if (!strcmp(cmd, "tree")) {
      viewSystemTree(peek(sysStack), 0);
    } else if (!strcmp(cmd, "cd")) {
      cmd = strtok(NULL, " ");
      strip(cmd, '\n');
      changeDir(cmd);
    } else if (!strcmp(cmd, "mkdir")) {
      cmd = strtok(NULL, " ");
      strip(cmd, '\n');
      makeDirectory(cmd);
    } else if (!strcmp(cmd, "touch")) {
      cmd = strtok(NULL, " ");
      strip(cmd, '\n');
      createFile(cmd);
    } else if (!strcmp(cmd, "cat")) {
      cmd = strtok(NULL, " ");
      strip(cmd, '\n');
      showFile(cmd);
    } else if (!strcmp(cmd, "vi")) {
      cmd = strtok(NULL, " ");
      strip(cmd, '\n');
      openFile(cmd);
    } else if (!strcmp(cmd, "rm")) {
      cmd = strtok(NULL, " ");
      strip(cmd, '\n');
      deleteFile(cmd);
    } else if (!strcmp(cmd, "rmdir")) {
      cmd = strtok(NULL, " ");
      strip(cmd, '\n');
      deleteFolder(cmd);
    } else if (!strcmp(cmd, "exit")) {
      break;
    } else if (!strcmp(cmd, "clear")) {
      write(STDOUT_FILENO, "\x1b[2J", 4); 
      write(STDOUT_FILENO, "\x1b[H", 3);  
    } else {
      printf("Error - Unknown command\n");
    }
    prompt(curr);
    cmd = NULL;
  }
  shutdownFileSystem(argv[1]);
  return 0;
}
