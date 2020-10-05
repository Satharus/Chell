#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>
#include <dirent.h>
#include <signal.h> 

#include "defs.h"
#include "chad-readline.h"
#include "chad-history.h"

struct executable
{
    char name[NAME_MAX + 1];
    char path[PATH_MAX + 1];
};

int getPATHLocationCount(char *PATH);
void getPATHLocations(char *directories[], char *PATH);

struct executable *getFilesFromDirectories(char **dir, int numberOfDirectory);


void initPrompt();

int splitString(char *split[], char *string, char *delim);
int splitCommand(char *argv[], char *command);

void executeCommand(char *commandString, struct executable *executables);

void sigintHandler(int signal_number);

char isWhiteSpaces(char *str);
