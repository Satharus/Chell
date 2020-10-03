#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#define VERSION 0.03

struct executable
{
    char name[40];
    char path[40];
};

int getPATHLocationCount(char *PATH);
void getPATHLocations(char *directories[], char *PATH);

int nExecutables = 0;
struct executable *getFilesFromDirectories(char **dir, int numberOfDirectory);


char *initPrompt();
char *createPromptString(char*, char*, char);

int splitString(char *split[], char *string, char *delim);
int splitCommand(char *argv[], char *command);

void executeCommand(char *commandString, struct executable *executables);

void cd(char *path);
