#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#ifndef CHELL_IN
#define CHELL_IN

int getch(void);
unsigned char detect_arrow(char c, unsigned char *arrow);
unsigned char handle_special(char c, char *current, size_t *cursor);

void getcommand(char **command, size_t *size);

#endif