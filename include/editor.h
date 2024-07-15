#ifndef EDITOR_H
#define EDITOR_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "sysfn.h"

typedef struct {
  char *data;
  size_t length;
  size_t capacity;
} Buffer;

void buffer_init(Buffer *buffer);
void buffer_append(Buffer *buffer, char c);
void buffer_insert(Buffer *buffer, size_t pos, char c);
void buffer_delete(Buffer *buffer, size_t pos);
void buffer_free(Buffer *buffer);

void disable_raw_mode();
void enable_raw_mode();
void read_file(char *filename, Buffer *buffer);
void write_file(char *filename, Buffer *buffer);
void display_buffer(char *file, Buffer *buffer, bool isSaved);
void editor(char *nodeName);
#endif
