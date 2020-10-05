#include "chad-readline.h"

/* Function to detect arrow key pressed in incoming stream
      Returns: 2 if finished checked, 1 if still checking, 0 if initially checked, -1 if there are no special characters
      arrow codes:
      0 -> up arrow
      1 -> down arrow
      2 -> right arrow
      3 -> left arrow 
*/ 
char escape_detected(char input, unsigned char *escapeCharacter)
{
    //Arrows are ANSI escapes that produce the sequence 27, 91, number
    //Where number is the value of the arrow itself.
    static unsigned char characterBeingChecked;

    if ((input == 27 && characterBeingChecked == 0) || (input == 91 && characterBeingChecked == 1))
    {
        return characterBeingChecked++;
    }
    else if (characterBeingChecked == 2) //Special key (ANSI ESC)
    {
        if ('A' <= input && input <= 'D')
        {
            *escapeCharacter = input - 'A';
        }
        else
            *escapeCharacter = 0xFF; //Not an arrow for now, could add more options later.
        characterBeingChecked = 0;
        return 2;
    }
    else
    {
        characterBeingChecked = 0;
    }
    return -1;
}

char handle_special(char input, char *line, int* cursor)
{
    if (input == 127) //Backspace
    {
        if (*cursor > 0)
        {
            line[--*cursor] = 0;
            printf("%s", "\b \b");
        }
    }
    else if (input == '\t') 
    {
        //code for autocomplete
    }
    else return 0;

    return 1;
}

char *readline(char *prompt)
{
    printf(prompt);
    char *line = (char*) malloc(sizeof(char)*ARG_MAX);
    int cursor = 0;
    char input;
    unsigned char escapeCharacter;

    while(input = getch())
    {
        char status = escape_detected(input, &escapeCharacter);
        if (status == 0 || status == 1) continue;
        else if(status ==  2) //Finished checking
        {
            if (escapeCharacter == LEFT_ARROW);  //Move Left
            if (escapeCharacter == RIGHT_ARROW); //Move right
            if (escapeCharacter == UP_ARROW) {   //History up
                char *cmd = historyHandler.getPrev(line);
                if(cmd != NULL) {
                    replaceCommandDisplay(prompt, cmd, line);
                }
                cursor = strlen(line);
            }
            if (escapeCharacter == DOWN_ARROW) { //History down
                char *cmd = historyHandler.getNext(line);
                // printf("\n%d -> %s\n", cmd, cmd);
                if(cmd != NULL) {
                    replaceCommandDisplay(prompt, cmd, line);
                }
                cursor = strlen(line);
            }
        }
        else
        {
            if (input == 0xA) //Enter
            {
                line[cursor] = '\0';
                printf("\n");
                historyHandler.add(line);
                return line;
            }
            else if(!handle_special(input, line, &cursor))
            {
                if (cursor < ARG_MAX)
                {
                    line[cursor] = input;
                    putchar(input);
                    cursor++;
                }
            }
        }
    }
}


void replaceCommandDisplay(char *prompt, char *command, char *line) {
    int len = strlen(prompt) + strlen(line);
    while(len--) printf("\b \b");
    printf("%s%s", prompt, command);
    strcpy(line, command);
}


/*
   The following is a small implementation for getch() taken from
   https://www.includehelp.com/c-programs/gotoxy-clrscr-getch-getche-for-gcc-linux.aspx
*/
void initTermios(int echo)
{
    tcgetattr(0, &old); // grab old terminal i/o settings
    newi = old; // make new settings same as old settings
    newi.c_lflag &= ~ICANON; // disable buffered i/o

    if (echo)
        newi.c_lflag |= ECHO; // set echo mode
    else
        newi.c_lflag &= ~ECHO; // set no echo mode

    tcsetattr(0, TCSANOW, &newi); // use these new terminal i/o settings now
}

char _getch(int echo)
{
    char ch;
    initTermios(echo);
    ch = getchar();
    //Restore old terminal i/o settings
    tcsetattr(0, TCSANOW, &old);
    return ch;
}

char getch(void)
{
    return _getch(0);
}

char getche(void)
{
    return _getch(1);
}

