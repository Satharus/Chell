#include "chell.h"

int main()
{
    int numberOfDirs = getPATHLocationCount(getenv("PATH"));
    char *PATHdirs[numberOfDirs];

    for (int i = 0; i < numberOfDirs; i++)
        PATHdirs[i] = (char*) malloc(sizeof(char *)*40);

    getPATHLocations(PATHdirs, getenv("PATH"));
    struct executable *executables = getFilesFromDirectories(PATHdirs, numberOfDirs);

    while (1)
    {
        initPrompt();

        size_t size = 256;
        char *command = (char*) malloc(sizeof(char)*size);

        //Command input
        getline(&command, &size, stdin);

        //Delete the newline
        command[strlen(command)-1] = 0;
        
        if (strcmp(command, "") != 0) 
            executeCommand(command, executables);

        free(command);
    }

    for (int i = 0; i < numberOfDirs; i++)
        free(PATHdirs[i]);
    free(executables);
}

int getPATHLocationCount(char *PATH)
{
    int j = 0;
    for (int i = 0; i < strlen(PATH); i++)
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
    struct executable *executables = malloc(numberOfDirectory*4000*sizeof(struct executable));
    
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
                if (!strcmp(ep->d_name, ".") || !strcmp(ep->d_name, ".."))
                    continue;
                
                strcpy(executables[j].name, ep->d_name);
                strcpy(executables[j].path, dir[i]);
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
    char *home = getenv("HOME");
    char *user = getenv("USER");
    char *pwd =  getenv("PWD");

    if (pwd == NULL) pwd = "";
    if (user == NULL) sprintf(user, "%s-v%.2f", "Chell", VERSION);

    if (strcmp(pwd, home) == 0)
        strcpy(pwd, "~");
    
    if (strstr(pwd, home) != NULL)
    {
        char *actualpath = strstr(pwd+6, "/");
        sprintf(pwd, "~%s", actualpath);
    }

    char prompt;
    if (strcmp(user, "root") == 0)
        prompt = '#';
    else
        prompt = '$';

    printf("%s:%s%c ", user, pwd, prompt);
}

int splitString(char *split[], char *string, char *delim)
{
    char *arg = strtok(string, delim);

    int argc = 0;
    while (arg != NULL)
    {
        strcpy(split[argc], arg);
        argc++;
        arg = strtok(NULL, delim);
    }

    return argc;

}

int splitCommand(char *argv[], char *command)
{
    return splitString(argv, command, "\t ");
}

void executeCommand(char *commandString, struct executable *executables)
{
    if (strcmp(commandString, "exit") == 0 || strcmp(commandString, "quit") == 0 || strcmp(commandString, "q") == 0)
        exit(0);
    
    pid_t processID;
    char *argv[256];
    
    //Allocate the argv array
    for (int i = 0; i < 256; i++)
        argv[i]= malloc(sizeof(char)*256);
    
    //Split the command and get the number of arguments
    int argc = splitCommand(argv, commandString);

    //Set the bounds for the last argument
    argv[argc] = NULL;

    //Get the actual command path
    char *commandPath = malloc(256*sizeof(char));
    char programExists = 0;
    for (int i = 0; i < nExecutables; i++)
    {
        if (strcmp(argv[0], executables[i].name) == 0)
        {
            sprintf(commandPath,"%s/%s", executables[i].path, executables[i].name);
            programExists = 1;
            break;
        }
    }
    
    if (programExists)
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
    else if (strcmp(argv[0], "cd") == 0)
    {
        if (argv[1] != NULL)
            cd(argv[1]);
        else
            cd(getenv("HOME"));
    }
    else if (!programExists)
    {
        printf("%s: Command not found.\n", argv[0]);
    }

    //Free the argv array
    for (int i = 0; i < 256; i++)
        free(argv[i]);      

    free (commandPath);
 
}

void cd(char *path)
{
    if (chdir(path) == 0) 
        setenv("PWD", getcwd(NULL, 4096), 1);
}
