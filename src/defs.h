#ifndef DEFS
#define DEFS

#define VERSION 0.05
#define SHELL_NAME "Chell"
#define HISTORY_SIZE 1024
#define HISTORY_FILE "/.chell_history"
#define ARG_MAX sysconf(_SC_ARG_MAX)

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define UP_ARROW        0
#define DOWN_ARROW      1
#define RIGHT_ARROW     2
#define LEFT_ARROW      3

#endif
