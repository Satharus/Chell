#include "chad-history.h"

struct history_manager {
    char **history_list;
    int history_index;
    unsigned int history_size;
};

struct history_manager old_history, new_history;

void initHistory() {
    history_file = (char *) malloc(sizeof(char)*(strlen(getenv("HOME")) + strlen(HISTORY_FILE) + 1));
    strcpy(history_file, getenv("HOME"));
    strcat(history_file, HISTORY_FILE);
    initNewHistory();
    loadHistory();
    historyHandler.getNext = *getNext;
    historyHandler.getPrev = *getPrev;
    historyHandler.add = *addHistory;
    buffer = (char *) malloc(sizeof(char)*ARG_MAX);
}

void initNewHistory() {
    new_history.history_list = calloc(HISTORY_SIZE, sizeof(char *));
    new_history.history_index = new_history.history_size = 0;
}

char *getNext(char *command) { // Test fail on first cmd to return buffer;
    if (new_history.history_index == -1) {
        if (old_history.history_index == old_history.history_size - 1) {
            old_history.history_index++;
            return new_history.history_list[++new_history.history_index];
        } else {
            return old_history.history_list[++old_history.history_index];
        }
    } else if (new_history.history_index < new_history.history_size - 1) {
        return new_history.history_list[++new_history.history_index];
    } else {
        if (new_history.history_index == new_history.history_size - 1) new_history.history_index++;
        // if(new_history.history_size == 0) return command;
        return buffer;
    }
}

char *getPrev(char *command) {
    if(new_history.history_index == new_history.history_size) strcpy(buffer, command);
    if (new_history.history_index == -1) {
        if(old_history.history_index > 0) old_history.history_index--;
        return old_history.history_list[old_history.history_index];
    } else if (new_history.history_index == 0) {
        if(old_history.history_size != 0) {
            new_history.history_index --;
            return old_history.history_list[--old_history.history_index];
        } else if (new_history.history_size != 0) {
            return new_history.history_list[new_history.history_index];
        } else {
            return buffer;
        }
    } else {
        return new_history.history_list[--new_history.history_index];
    }
}

void addHistory(char *command) {
    if(new_history.history_size == HISTORY_SIZE) reloadHistory();
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
