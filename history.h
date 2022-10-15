#ifndef __HISTORY_H
#define __HISTORY_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HISTORYFILE ".bodashell_history"
#define HISTORY_BUFF_SIZE 256

typedef struct HISTORY_t
{
    FILE *history_fd;
    char **history_commands;
    int history_total_commands;
    int current_index;
    char *path;

}SHELL_HISTORY;

int init_history(SHELL_HISTORY **history, char *home_dir);
int load_history(SHELL_HISTORY *history);
void destroy_history(SHELL_HISTORY **history);
void browse_history_up(SHELL_HISTORY *history);
void browse_history_down(SHELL_HISTORY *history);

#endif
