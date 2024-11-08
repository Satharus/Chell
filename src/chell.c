#include "chell.h"
#include "commands.h"

static int nExecutables = 0;

int main()
{
    signal(SIGINT, sigintHandler);
    int numberOfDirs = getPATHLocationCount(getenv("PATH"));
    char *PATHdirs[numberOfDirs];

    for (int i = 0; i < numberOfDirs; i++)
        PATHdirs[i] = (char*) malloc(sizeof(char *)*PATH_MAX);

    getPATHLocations(PATHdirs, getenv("PATH"));
    struct executable *executables = getFilesFromDirectories(PATHdirs, numberOfDirs);

    // Initialize command history handling functions
    initHistory();

    char *command;
    char status = 0;
    while (status == 0)
    {
        initPrompt();
        //Command input
        command = readline("");  

        //If not an empty command
        if (command && strlen(command) != 0 && !isWhiteSpaces(command))
            status = executeCommand(command, executables);
    }

    free(command);

    for (int i = 0; i < numberOfDirs; i++)
        free(PATHdirs[i]);

    free(executables);
}

int getPATHLocationCount(char *PATH)
{
    int j = 0;
    int pathLen = strlen(PATH); 
    for (int i = 0; i < pathLen; i++)
    {
        if (PATH[i] == ':') j++;
    }
    return j+1;
}

void getPATHLocations(char *directories[], char *PATH)
{
    splitString(directories, PATH, ":");
}

struct executable *getFilesFromDirectories(char **dir, int numberOfDirectory)
{
    struct executable *executables = malloc(numberOfDirectory*8192*sizeof(struct executable));

    int j = 0;
    for(int i = 0; i < numberOfDirectory; i++)
    {
        DIR *dirPointer;
        struct dirent *ep;
        dirPointer = opendir(dir[i]);

        if (dirPointer != NULL)
        {
            while ((ep = readdir(dirPointer)))
            {
                if (!strncmp(ep->d_name, ".", 1) || !strncmp(ep->d_name, "..", 1))
                    continue;

                strncpy(executables[j].name, ep->d_name, strlen(ep->d_name));
                strncpy(executables[j].path, dir[i], strlen(dir[i]));
                j++;
            }
            (void) closedir (dirPointer);
        }
    }
    nExecutables = j;

    return executables;
}

void initPrompt()
{
    //Get environment variables for the prompt
    char *home = getenv("HOME");
    char *user = getenv("USER");
    char *pwd =  getenv("PWD");

    //If any of them are undefined
    if (pwd == NULL) strncpy(pwd, "", 1);
    if (home == NULL)
    {
        if (user == NULL)
            strncpy(home, "", 1);
        else
        {
            snprintf(home, strlen(user) + 6, "/home/%s", user);
            setenv("HOME", home, 1);
        }
    }
    if (user == NULL) snprintf(user, 16, "%s-v%.2f", SHELL_NAME, VERSION);

    //If the current working dir is home
    if (strncmp(pwd, home, max(strlen(pwd), strlen(home))) == 0)
        strncpy(pwd, "~", strlen(pwd));

    //If the current working dir has /home/USER in it
    if (strstr(pwd, home) != NULL)
    {
        //Skip /home/user
        char *actualpath = strstr(pwd+strlen(user), "/");
        snprintf(pwd, strlen(actualpath) + 3, "~%s", actualpath);
    }

    char prompt;
    if (getuid() == 0)
        prompt = '#';
    else
        prompt = '$';

    printf("%s:%s%c ", user, pwd, prompt);
}

int splitString(char *split[], char *string, char *delim)
{
    char *stringDup = strdup(string);
    char *arg = strtok(stringDup, delim);

    int argc = 0;
    while (arg != NULL)
    {
        strncpy(split[argc], arg, PATH_MAX);
        argc++;
        arg = strtok(NULL, delim);
    }
    free(stringDup);
    return argc;
}

int splitCommand(char *argv[], char *command)
{
    return splitString(argv, command, "\t ");
}

char executeCommand(char *commandString, struct executable *executables)
{
    if (strlen(commandString) == 0 || commandString == NULL)
        return 0;

    if (strncmp(commandString, "exit", strlen(commandString)) == 0 ||
        (strncmp(commandString, "quit", strlen(commandString)) == 0  && strlen(commandString) == 4)||
        (strncmp(commandString, "q", strlen(commandString)) == 0 && strlen(commandString) == 1))
    {
        saveHistory();
        freeHistory();
        return 1;
    } 

    pid_t processID;
    char *argv[PATH_MAX];
    struct command* builtin;

    //Allocate the argv array
    for (int i = 0; i < PATH_MAX; i++)
        argv[i] = malloc(sizeof(char)*PATH_MAX);

    //Split the command and get the number of arguments
    int argc = splitCommand(argv, commandString);

    //Set the bounds for the last argument
    free(argv[argc]);
    argv[argc] = NULL;

    //Get the actual command path
    char *commandPath = malloc(PATH_MAX*sizeof(char));
    char programExists = 0;

    // Check if it's a relative / full path first
    if (realpath(argv[0], commandPath) != NULL && strstr(argv[0], "/") != NULL)
    {
        programExists = 1;
    }
    else
    {
        for (int i = 0; i < nExecutables; i++)
        {
            if (strncmp(argv[0], executables[i].name, max(strlen(executables[i].name), strlen(argv[0]))) == 0)
            {
                snprintf(commandPath, NAME_MAX + PATH_MAX + 2,"%s/%s", executables[i].path, executables[i].name);
                programExists = 1;
                break;
            }
        }
    }

    // check if the executed command is built into Chell
    if ((builtin = is_builtin(argv[0])))
    {
        if (argv[1] != NULL && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))
            printf("\t%s - %s\n", builtin->name, builtin->help);
        else
        {
            builtin->func(argv[1]);

            if (builtin->returnsValue)
            {
                if (strcmp(argv[0], "history") == 0 && argv[1] != NULL)
                {
                    executeCommand(argv[1], executables);
                }
            }
        }
    }
    else if (programExists)
    {
        processID = fork();
        if (processID == 0)
        {
            //Create a new process for the command
            execv(commandPath, argv);
            exit(0);
        }
        else
            waitpid(processID, 0, 0);
    }
    else if (!programExists)
    {
        printf("%s: Command not found.\n", argv[0]);
    }

    //Free the argv array
    for (int i = 0; i < PATH_MAX; i++)
        free(argv[i]);

    free (commandPath);
    free (commandString);
    return 0;

}

void sigintHandler(int signal_number)
{
    signal(SIGINT, sigintHandler);
    printf("\n");
    initPrompt();
    clearBuffer();
    fflush(stdout);
}


