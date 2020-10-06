#include "commands.h"

struct command commands[] = {
    { "cd", cd, "Change directory" },
    { NULL, NULL, NULL}
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

