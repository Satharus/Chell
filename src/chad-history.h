#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "defs.h"

#ifndef CHAD_HISTORY
#define CHAD_HISTORY

struct history_handler
{
    char *(*getNext)();
    char *(*getPrev)();
    void (*add)(char *);
};

void constructHistoryFilePath();
void initHistory(void);
void loadHistory(void);
void saveHistory(void);
void freeHistory();

char *getNext(char *command);
char *getPrev(char *command);
void addHistory(char *command);
extern struct history_handler historyHandler;
#endif
