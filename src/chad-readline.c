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

char handle_special(char input, char *line, int* cursor, int *currentLength)
{
    //TODO: Make using backspace more natural, the feature feels wrong right now.
    if (input == 127) //Backspace
    {
        if (*cursor > 0)
        {
            if ((*cursor) == (*currentLength)-1)
            {
                line[--*cursor] = '\0';
                (*currentLength)--;
                printf("%s", "\b \b");
            }
            else //not at the end
            {
                //Shift all to the left
                for (int i = *cursor; i < *currentLength; i++) 
                    line[i-1] = line[i];       

                //decrement the length 
                (*currentLength)--;

                //calculate the number of movements needed forward
                int forward = *currentLength - *cursor; 
                //move to the right 
                for (int i = 0; i < forward; i++) 
                { 
                    printf("%c%c%c", 27, 91, RIGHT_ARROW + 'A'); 
                }  
                //go back and delete on the way
                for (int i = 0; i < *currentLength; i++)  
                { 
                    printf("%s", "\b \b");
                } 

                //move the cursor back
                (*cursor)--;

                //print the new line
                printf("%.*s", (*currentLength)-1, line);

                //Go back to the original cursor position
                for (int i = 0; i < forward; i++) 
                    printf("%c%c%c", 27, 91, LEFT_ARROW + 'A'); 
            }
        }
    }
    else if (input == '\t') 
    {
        //code for autocomplete
    }
    else return 0;

    return 1;
}

int cursor = 0; //Where the cursor is respective to line.
int currentLength = 1;
void clearBuffer()
{
    cursor = 0;
    currentLength = 1;
}

char *readline(char *prompt)
{
    printf(prompt);
    char *line = (char*) malloc(sizeof(char)*ARG_MAX);   //Current input
    
    cursor = 0;
    currentLength = 1;

    char input;
    unsigned char escapeCharacter;

    while(input = getch())
    {
        char status = escape_detected(input, &escapeCharacter);
        if (status == 0 || status == 1) continue;
        else if(status ==  2) //Finished checking
        {
            if (escapeCharacter == LEFT_ARROW)  //Move Left
            {
                if (cursor > 0)
                {
                    cursor--;
                    printf("%c%c%c", 27, 91, LEFT_ARROW + 'A');
                }
                
            }
            else if (escapeCharacter == RIGHT_ARROW) //Move right
            {
                if (cursor < currentLength-1)
                {
                    printf("%c%c%c", 27, 91, RIGHT_ARROW + 'A');
                    cursor++;
                }
            }
            if (escapeCharacter == UP_ARROW);    //History up
            if (escapeCharacter == DOWN_ARROW);  //History down
        }
        else
        {
            if (input == 0xA) //Enter
            {
                line[currentLength-1] = '\0';
                printf("\n");
                clearBuffer();
                return line;
            }
            else if(!handle_special(input, line, &cursor, &currentLength))
            {
                if (cursor < ARG_MAX)
                {
                    if (cursor == currentLength-1)
                    {                    
                        line[cursor] = input;
                        putchar(input);
                        currentLength++;
                        cursor++;
                    }
                    else
                    {
                        //Save the location of the cursor
                        printf("%c%c%c", 27, 91, 's');
                        //Shift all of the characters after the cursor to the right.
                        for (int i = currentLength; i > cursor; i--)
                            line[i] = line[i-1];

                        //Increment the length for the newly added character.
                        currentLength++;

                        //Place the new character.
                        line[cursor] = input;
                        
                        //print the rest of the new characters
                        for (int i = cursor; i < currentLength; i++)
                            putchar(line[i]);
                        
                        //Load the location of the cursor
                        printf("%c%c%c", 27, 91, 'u');

                        //Emulate a right key press
                        printf("%c%c%c", 27, 91, RIGHT_ARROW + 'A');
                        cursor++;
                    }
                }
            }
        }
    }
}

// //1 -> right, 0 -> left
// void shiftAllCharacter(char direction, char *str, int startIndex, int lastIndex, char toInsert)
// {
//     
// }

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

