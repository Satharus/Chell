#include "commands.h"
#include "chad-history.h"

struct command commands[] = {
    { "cd", cd, "change directory\n\tUsage: cd [path]\n\n\t\tChanges the working directory to the specified path.\n", 0},
    { "history", historyCommand, "manage the history\n\n\tUsage: history [arg]\n\t\tPrints or clears the history. It can also execute a previous command.\n\t\targ can be:\n\t\t\tclear - clear the history entirely.\n\t\t\tn - execute command number \"n\" from the history.", 1},
    { NULL, NULL, NULL, 0}
};

struct command *is_builtin(char *name)
{
    for (int i = 0; commands[i].name; ++i)
    {
        if (strcmp(name, commands[i].name) == 0)
            return &commands[i];
    }

    return NULL;
}

void cd(char *path)
{
    if (path == NULL)
    {
        path = getenv("HOME");
    }

    int status = chdir(path);

    if (status == 0)
        setenv("PWD", getcwd(NULL, 0), 1);
    else if (status == -1)
    {
        if (errno == EACCES)
            printf("%s: Permission denied.\n", path);
        else if (errno == ENOENT)
            printf("%s: Doesn't exist.\n", path);
        else if (errno == ENOTDIR)
            printf("%s: Not a directory.\n", path);
    }
}

