#ifndef DEFS
#define DEFS

#define VERSION 0.07
#define SHELL_NAME "Chell"
#define HISTORY_SIZE 1024
#define HISTORY_FILE "/.chell_history"

#ifndef ARG_MAX // it's already defined in sys/syslimits.h on macos
#define ARG_MAX sysconf(_SC_ARG_MAX)
#endif

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define UP_ARROW        0
#define DOWN_ARROW      1
#define RIGHT_ARROW     2
#define LEFT_ARROW      3
#define HOME            7
#define END             5
#define INSERT          0x32    //The character '2' in <esc>[2~
#define DELETE          0x33    //The character '3' in <esc>[3~

#endif
