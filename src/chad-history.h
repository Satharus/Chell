#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "defs.h"

#ifndef CHAD_HISTORY
#define CHAD_HISTORY

char *history_file;

struct history_handler {
    char *(*getNext)(char *);
    char *(*getPrev)(char *);
    void (*add)(char *);
} historyHandler;

char *buffer;

void initHistory(void);
void loadHistory(void);
void saveHistory(void);
void reloadHistory(void);
void initNewHistory(void);

char *getNext(char *command);
char *getPrev(char *command);
void addHistory(char *command);

extern struct history_handler historyHandler;
#endif
