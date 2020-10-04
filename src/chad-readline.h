#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include "defs.h"


/* Function to detect arrow key pressed in incoming stream
      Returns: 2 if finished checked, 1 if still checking, 0 if initially checked, -1 if there are no special characters
      arrow codes:
      0 -> up arrow
      1 -> down arrow
      2 -> right arrow
      3 -> left arrow 
*/ 
char escape_detected(char input, unsigned char *escapeCharacter);

//Returns 1 if handled a key, returns 0 otherwise.
char handle_special(char input, char *line, int* cursor);

char *readline(char *prompt);

static struct termios old, newi;

// Initialize new terminal i/o settings 
void initTermios(int echo);

// Read 1 character - echo defines echo mode
char _getch(int echo);

// Read 1 character without echo
char getch(void);

// Read 1 character with echo
char getche(void);
