#include "chad-history.h"

struct history_manager {
    char **history_list;
    int history_index;
    unsigned int history_size;
};

struct history_manager old_history, new_history;

void initHistory() {
    history_file = (char*) malloc(strlen(getenv("HOME")) + strlen(HISTORY_FILE) + 1);
    strcpy(history_file, getenv("HOME"));
    strcat(history_file, HISTORY_FILE);
    initNewHistory();
    loadHistory();
    handler.next = *getNext;
    handler.prev = *getPrev;
    handler.add = *addHistory;
}

void initNewHistory() {
    new_history.history_list = calloc(BATCH_SIZE, sizeof(char *));
    new_history.history_index = new_history.history_size = 0;
}

char *getNext() {
    char *command = (char *) malloc(sizeof(char)*ARG_MAX);
    if (new_history.history_index == -1) {
        if (old_history.history_index == old_history.history_size-1) {
            old_history.history_index++;
            strcpy(command, new_history.history_list[++new_history.history_index]);
        } else {
            strcpy(command, old_history.history_list[++old_history.history_index]);
        }
    } else if (new_history.history_index != new_history.history_size) {
        strcpy(command, new_history.history_list[++new_history.history_index]);
    } else {
        free(command);
        command = NULL;
    }
    return command;
}

char *getPrev() {
    char *command = (char *) malloc(sizeof(char)*ARG_MAX);
    if (new_history.history_index == -1) {
        if(old_history.history_index > 0) old_history.history_index--;
        strcpy(command, old_history.history_list[old_history.history_index]);
    } else if (new_history.history_index == 0) {
        if(old_history.history_size != 0) {
            new_history.history_index --;
            strcpy(command, old_history.history_list[--old_history.history_index]);
        } else if (new_history.history_size != 0) {
            strcpy(command, new_history.history_list[new_history.history_index]);
        } else {
            free(command);
            command = NULL;
        }
    } else {
        strcpy(command, new_history.history_list[--new_history.history_index]);
    }
    return command;
}

void addHistory(char *command) {
    if(new_history.history_size == BATCH_SIZE) reloadHistory();
    new_history.history_list[new_history.history_size++] = command;
    new_history.history_index = new_history.history_size;
    old_history.history_index = old_history.history_size;
}

void loadHistory() {
    FILE *file;
    unsigned int count = 0;

    file = fopen(history_file, "r");
    if(file != NULL) {
        char c = fgetc(file);
        while(c != EOF) {
            if(c == '\n') count ++;
            c = fgetc(file);
        }

        old_history.history_list = calloc(count, sizeof(char *));
        old_history.history_size = count;
        fseek(file, 0, SEEK_SET);
        for(old_history.history_index = 0; old_history.history_index<count; old_history.history_index++) {
            char *line = (char *) malloc(sizeof(char)*ARG_MAX);
            int index = 0;
            c = fgetc(file);
            while(c != '\n') {
                line[index++] = c;
                c = fgetc(file);
            }
            old_history.history_list[old_history.history_index] = line;
        }
        fclose(file);
    }
}

void saveHistory() {
    FILE *file;
    file = fopen(history_file, "a");
    if(file != NULL) {
        for(int i=0; i<new_history.history_size; i++) {
            fprintf(file, "%s\n", new_history.history_list[i]);
        }
        fclose(file);
    }
    free(new_history.history_list);
}

void reloadHistory() {
    saveHistory();
    while(old_history.history_size --) {
        free(old_history.history_list[old_history.history_size]);
    }
    free(old_history.history_list);
    initNewHistory();
    loadHistory();
}