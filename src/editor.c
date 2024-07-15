#include "../include/editor.h"

#include <stdlib.h>

#define INITIAL_CAPACITY 1000
#define GROWTH_FACTOR 2
#define CTRL_KEY(k) ((k) & 0x1f)

struct termios orig_termios;

void buffer_init(Buffer *buffer) {
  buffer->data = malloc(INITIAL_CAPACITY);
  if (buffer->data == NULL) {
    perror("Failed to allocate memory");
    exit(1);
  }
  buffer->length = 0;
  buffer->capacity = INITIAL_CAPACITY;
}

void buffer_append(Buffer *buffer, char c) {
  if (buffer->length + 1 >= buffer->capacity) {
    size_t new_capacity = buffer->capacity * GROWTH_FACTOR;
    char *new_data = realloc(buffer->data, new_capacity);
    if (new_data == NULL) {
      perror("Failed to reallocate memory");
      exit(1);
    }
    buffer->data = new_data;
    buffer->capacity = new_capacity;
  }
  buffer->data[buffer->length++] = c;
  buffer->data[buffer->length] = '\0';
}

void buffer_insert(Buffer *buffer, size_t pos, char c) {
  buffer_append(buffer, '\0');  // This ensures we have space
  memmove(&buffer->data[pos + 1], &buffer->data[pos], buffer->length - pos);
  buffer->data[pos] = c;
}

void buffer_delete(Buffer *buffer, size_t pos) {
  if (pos < buffer->length) {
    memmove(&buffer->data[pos], &buffer->data[pos + 1], buffer->length - pos);
    buffer->length--;
  }
}

void buffer_free(Buffer *buffer) {
  free(buffer->data);
  buffer->data = NULL;
  buffer->length = 0;
  buffer->capacity = 0;
}

void disable_raw_mode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disable_raw_mode);

  struct termios raw = orig_termios;
  raw.c_iflag &= ~(ICRNL | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void read_file(char *filename, Buffer *buffer) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error opening file. Starting with empty buffer.\n");
    return;
  }
  int c;
  while ((c = fgetc(file)) != EOF) {
    buffer_append(buffer, c);
  }
  fclose(file);
}

void write_file(char *filename, Buffer *buffer) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    printf("Error opening file for writing.\n");
    return;
  }
  fwrite(buffer->data, 1, buffer->length, file);
  fclose(file);
  printf("\r\nFile saved.\r\n");
}

void display_buffer(char *file, Buffer *buffer, bool isSaved) {
  write(STDOUT_FILENO, "\x1b[2J", 4);  // Clear the screen
  write(STDOUT_FILENO, "\x1b[H", 3);   // Move cursor to home position

  char flag = '*';
  if (isSaved) flag = ' ';
  printf("Press Ctrl-S to save, Ctrl-Q to exit.\r\nEditing file %s%c\r\n\r\n", file, flag);

  printf("%s", buffer->data);
  fflush(stdout);
}

void editor(char *fileName) {
  Buffer buffer;
  buffer_init(&buffer);
  size_t cursor_pos = 0;
  int c;

  read_file(fileName, &buffer);
  cursor_pos = buffer.length;

  enable_raw_mode();
  display_buffer(fileName, &buffer, true);

  while (1) {
    c = getchar();
    if (c == CTRL_KEY('s')) {
      write_file(fileName, &buffer);
      display_buffer(fileName, &buffer, true);
    } else if (c == CTRL_KEY('q')) {
      printf("\r\nExiting...\r\n");
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
      break;
    } else if (c == 127) {  // Backspace
      if (cursor_pos > 0) {
        cursor_pos--;
        buffer_delete(&buffer, cursor_pos);
        display_buffer(fileName, &buffer, false);
      }
    } else if (c == 13) {  // Enter key
      buffer_insert(&buffer, cursor_pos, '\r');
      cursor_pos++;
      buffer_insert(&buffer, cursor_pos, '\n');
      cursor_pos++;
      display_buffer(fileName, &buffer, false);
    } else if (isprint(c) || c == '\t') {
      buffer_insert(&buffer, cursor_pos, c);
      cursor_pos++;
      display_buffer(fileName, &buffer, false);
    }
  }
  buffer_free(&buffer);
  disable_raw_mode();
}
