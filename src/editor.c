#include "../include/editor.h"

#define MAX_LENGTH 1000
#define CTRL_KEY(k) ((k) & 0x1f)

struct termios orig_termios;

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

void read_file(char *filename, char *content) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error opening file. Starting with empty buffer.\n");
    content[0] = '\0';
    return;
  }
  fread(content, 1, MAX_LENGTH - 1, file);
  content[MAX_LENGTH - 1] = '\0';
  fclose(file);
}

void write_file(char *filename, char *content) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    printf("Error opening file for writing.\n");
    return;
  }
  fputs(content, file);
  fclose(file);
  printf("\r\nFile saved.\r\n");
}

void display_buffer(char *file, char *buffer, bool isSaved) {
  write(STDOUT_FILENO, "\x1b[2J", 4);  // Clear the screen
  write(STDOUT_FILENO, "\x1b[H", 3);   // Move cursor to home position

  char flag = '*';
  if (isSaved) flag = ' ';
  printf("Press Ctrl-S to save, Ctrl-Q to exit.\r\nEditing file %s%c\r\n\r\n", file, flag);

  printf("%s", buffer);
  fflush(stdout);
}

void editor(char *fileName) {
  char buffer[MAX_LENGTH] = {0};
  int buf_pos = 0;
  int c;

  read_file(fileName, buffer);
  buf_pos = strlen(buffer);

  enable_raw_mode();
  display_buffer(fileName, buffer, true);

  while (1) {
    c = getchar();
    if (c == CTRL_KEY('s')) {
      write_file(fileName, buffer);
      display_buffer(fileName, buffer, true);
    } else if (c == CTRL_KEY('q')) {
      printf("\r\nExiting...\r\n");
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
      break;
    } else if (c == 127) {  // Backspace
      if (buf_pos > 0) {
        buf_pos--;
        memmove(&buffer[buf_pos], &buffer[buf_pos + 1], strlen(buffer) - buf_pos);
        display_buffer(fileName, buffer, false);
      }
    } else if (c == 13) {  // Enter key
      if (buf_pos < MAX_LENGTH - 2) {
        memmove(&buffer[buf_pos + 1], &buffer[buf_pos], strlen(buffer) - buf_pos + 1);
        buffer[buf_pos++] = '\r';
        buffer[buf_pos++] = '\n';
        display_buffer(fileName, buffer, false);
      }
    } else if (isprint(c) || c == '\t') {
      if (buf_pos < MAX_LENGTH - 1) {
        memmove(&buffer[buf_pos + 1], &buffer[buf_pos], strlen(buffer) - buf_pos + 1);
        buffer[buf_pos++] = c;
        display_buffer(fileName, buffer, false);
        (buffer);
      }
    }
  }
  disable_raw_mode();
}
