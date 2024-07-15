#ifndef EDITOR_H
#define EDITOR_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>

#include "sysfn.h"

void disable_raw_mode();
void enable_raw_mode();
void read_file(char *filename, char *content);
void write_file(char *filename, char *content);
void display_buffer(char *file, char *buffer, bool isSaved);
void editor(char *nodeName);

#endif
