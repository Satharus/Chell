#include "chellin.h"

unsigned char detector_index;

void getcommand(char **command, size_t *size) {
    char *current = (char *) malloc(sizeof(char)*(*size));
    size_t cursor = 0;
    char c;
    unsigned char arrow = 0;
    detector_index = 0;
    while(c = getch())
    {
        if(detect_arrow(c, &arrow)) {
            if(detector_index == 0) {
                switch(arrow)
                {
                    case 0:
                        break;
                    case 1:
                        break;
                }
            }
        } else {
            if(c == 10) { // Return key
                *(current + cursor) = 0;
                printf("\n");
                strcpy(*command, current);
                break;
            } else if(!handle_special(c, current, &cursor)) {
                *(current + cursor) = c;
                printf("%c", c);
                if(cursor < *size - 1) cursor ++;
            }
        }
    }
    free(current);
}

/* Function to handle special characters 
 * Returns: If specail character input handled -> 1, else 0 */
unsigned char handle_special(char c, char *current, size_t *cursor) {
    switch(c) {
        case 127: // Backspace key
            if (*cursor > 0) {
                *(current + --(*cursor)) = 0;
                printf("%c%c%c", 0x08,' ', 0x08);
            }
            break;
        case '\t': // Tab key
        case 27: // Esc key
            break;
        default:
            return 0;
    }
    return 1;
}

/* Function to detect arrow key pressed in incoming stream
 * Returns: if arrow key detected -> 1, else 0
 * arrow codes:
 * 0 -> up arrow
 * 1 -> down arrow
 * 2 -> right arrow
 * 3 -> left arrow */ 
unsigned char detect_arrow(char c, unsigned char *arrow) {
    if ((c == 27 && detector_index == 0) || (c == 91 && detector_index == 1)) {
        detector_index ++;
        return 1;
    } else if (c >= 'A' && c <= 'D' && detector_index == 2) {
        // This condition can be modified to detect all special keys
        *arrow = c - 'A';
        detector_index = 0;
        return 1;
    } else {
        detector_index = 0;
    }
    return 0;
}

/* code for getch() from conio.h */
int getch(void) {
      int c=0;

      struct termios org_opts, new_opts;
      int res=0;
          //-----  store old settings -----------
      res=tcgetattr(STDIN_FILENO, &org_opts);
      assert(res==0);
          //---- set new terminal parms --------
      memcpy(&new_opts, &org_opts, sizeof(new_opts));
      new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
      tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
      c=getchar();
          //------  restore old settings ---------
      res=tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
      assert(res==0);
      return(c);
}