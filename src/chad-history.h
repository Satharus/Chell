#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "defs.h"

#define BATCH_SIZE 1024
#define HISTORY_FILE "/.chell_history"

char *history_file;

struct history_handler {
    char *(*next)();
    char *(*prev)();
    void (*add)(char *);
} handler;

void initHistory(void);
void loadHistory(void);
void saveHistory(void);
void reloadHistory(void);
void initNewHistory(void);

char *getNext(void);
char *getPrev(void);
void addHistory(char *command);

extern struct history_handler handler;