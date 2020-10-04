#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

struct command {
    char *name;
    void (*func)(char*);
    char *help;
};

struct command *is_builtin(char *name);
void cd(char *path);

extern struct command commands[];