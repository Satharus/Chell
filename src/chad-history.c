#include "chad-history.h"
#include "chell.h"


char *history_file = (char *)NULL;
int history_file_length;

struct history_handler historyHandler = {NULL, NULL, NULL};

struct history_manager
{
    char **history_list;
    int history_index;
    unsigned int history_size;
    char *current_buffer;
};

struct history_manager history;

void constructHistoryFilePath()
{
    history_file_length = strlen(getenv("HOME")) + strlen(HISTORY_FILE) + 1;
    history_file = (char *) malloc(sizeof(char)*history_file_length);
    strncpy(history_file, getenv("HOME"), strlen(getenv("HOME")));
    strncat(history_file, HISTORY_FILE, strlen(HISTORY_FILE) + 1);
}

void initHistory()
{
    constructHistoryFilePath();

    history.history_list = calloc(HISTORY_SIZE, sizeof(char *));
    history.history_index = 0;
    history.history_size = 0;
    history.current_buffer = (char *) malloc(sizeof(char)*ARG_MAX);
    strcpy(history.current_buffer, "");
    loadHistory();

    historyHandler.getNext = *getNext;
    historyHandler.getPrev = *getPrev;
    historyHandler.add = *addHistory;
}

char *getNext(char *command)
{
    if (history.history_index < history.history_size)
        history.history_index++;

    return (history.history_index == history.history_size) ? history.current_buffer : history.history_list[history.history_index];
}

char *getPrev(char *command)
{
    if(history.history_index == history.history_size) strcpy(history.current_buffer, command);
    if (history.history_index > 0)
        history.history_index--;


    return (history.history_index == history.history_size) ? history.current_buffer : history.history_list[history.history_index];
}

void addHistory(char *command)
{
    history.history_size %= HISTORY_SIZE;

    // Not saving if successively repeated command
    if(!(history.history_size != 0 && strcmp(history.history_list[(history.history_size - 1)%HISTORY_SIZE], command) == 0)) {
        if (history.history_list[history.history_size] == NULL)
            history.history_list[history.history_size] = (char *) malloc(sizeof(char)*ARG_MAX);
        strcpy(history.history_list[history.history_size], command);
        history.history_size++;
    }

    history.history_index = history.history_size;
    strcpy(history.current_buffer, "");
}

void loadHistory()
{
    FILE *file;

    file = fopen(history_file, "r");
    if(file != NULL)
    {
        while(1)
        {
            history.history_index = history.history_size;
            history.history_list[history.history_index] = (char *) malloc(sizeof(char)*ARG_MAX);
 
            if (fgets(history.history_list[history.history_index], ARG_MAX, file))
            {
                //Remove the '\n' character from the string
                history.history_list[history.history_index][strlen(history.history_list[history.history_index])-1] = '\0';
 
                history.history_size++;
            }
            else break;
        }
        fclose(file);
    }
}

void saveHistory()
{
    FILE *file;
    file = fopen(history_file, "w");
    if(file != NULL)
    {
        for (int i = history.history_size; i < HISTORY_SIZE; i++)
        {
            if (history.history_list[i] != NULL)
                fprintf(file, "%s\n", history.history_list[i]);

        }
        for (int i = 0; i < history.history_size; i++)
        {
            if (history.history_list[i] != NULL)
                fprintf(file, "%s\n", history.history_list[i]);
        }
        fclose(file);
    }
}

void freeHistory()
{
    for (int i = 0; i < history.history_size; i++)
        free(history.history_list[i]);

    free(history.history_list);
    free(history_file);
}

char isNumeric(char *str)
{
    int len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        if (!isdigit(str[i]))
            return 0;
    }
    return 1;
}

void historyCommand(char *arg)
{
    if (arg == NULL)
    {
        printf("History Size: %d\n", history.history_size);

        for (int i = 0; i < history.history_size; i++)
            printf("%d:\t%s\n", i+1, history.history_list[i]);
        return;
    }
    else
    {
        if (strncmp(arg, "clear", 5) == 0)
        {
            FILE *file;
            file = fopen(history_file, "w");
            if(file != NULL)
                fclose(file);

            freeHistory();
            initHistory();
            printf("%s\n", "History cleared.");
        }

        if (isNumeric(arg))
        {
            int index = atoi(arg);
            if (index > 0 &&  index < HISTORY_SIZE)
                strcpy(arg, history.history_list[index-1]);
        }
    }



}
