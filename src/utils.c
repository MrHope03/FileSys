#include "../include/utils.h"

void printIndent(int depth) {
  for (int i = 0; i < depth; i++) {
    printf("     ");
  }
}

void strip(char *str, char delim) {
  if (str[strlen(str) - 1] == '\n')
    str[strlen(str) - 1] = '\0';
}
