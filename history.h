#ifndef __HISTORY_H__
#define __HISTORY_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define HISTORYFILE ".bodashell_history"
#define HISTORY_BUFF_SIZE 256

typedef struct HISTORY_t
{
    FILE *hst_descriptor;
    char **history_commands;
    int history_lines;
    char *path;


}SHELL_HISTORY;

int save_command(char *buffer, SHELL_HISTORY *history);
int init_history(SHELL_HISTORY **history, char *home_dir_path);
int load_history(SHELL_HISTORY *history);
void free_history(SHELL_HISTORY *);
#endif