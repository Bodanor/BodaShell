#include "history.h"

int init_history(SHELL_HISTORY **history, char *home_dir)
{
    (*history) = (SHELL_HISTORY*)malloc(sizeof(SHELL_HISTORY));
    if ((*history) == NULL){
        perror("HISTORY INIT FAILED !\n");
        return -1;
    }

    (*history)->path = (char*)malloc(sizeof(char)*strlen(home_dir) + strlen(HISTORYFILE) + 2);
    if ((*history)->path == NULL){
        perror("PATH INIT FAILED !\n");
        return -1;
    }

    strcpy((*history)->path, home_dir);
    strcat((*history)->path, "/");
    strcat((*history)->path, HISTORYFILE);

    (*history)->history_commands = NULL;
    (*history)->history_total_commands = 0;
    (*history)->current_index = 0;

    return 0;
}

int load_history(SHELL_HISTORY *history)
{
    int c, step, i, tmp_step;
    char *buffer, *buffer_pt;

    step = 2;
    i = 0;
    buffer = NULL;

    buffer = (char*)malloc(sizeof(char)* HISTORY_BUFF_SIZE);
    if (buffer == NULL){
        perror("MALLOC ERROR !\n");
        return -1;
    }
    history->history_commands = (char**)malloc(sizeof(char*)*HISTORY_BUFF_SIZE);
    if (history->history_commands == NULL){
        perror("MALLOC ERROR !\n");
        return -1;
    }
    if ((history->history_fd = fopen(history->path, "r")) == NULL){
        perror("COULDN'T READ HISTORY FILE ");
        return -1;
    }

    buffer_pt = buffer;
    tmp_step = step;

    while ((c = fgetc(history->history_fd)) != EOF){
        if ((i + 1) % HISTORY_BUFF_SIZE == 0){
            buffer = (char*)realloc(buffer, sizeof(char*)*HISTORY_BUFF_SIZE *step);
            if (buffer == NULL){
                perror("REALLOC ERROR !\n");
                return -1;
            }
            buffer_pt = buffer + (i *(step-1));
            step++;
            i = 0;
        }
        if (c == '\n')
        {
            buffer_pt[i] = '\0';
            
            step = tmp_step;

            if ((history->history_total_commands + 1) % HISTORY_BUFF_SIZE == 0)
            {
                history->history_commands = (char**)realloc(history->history_commands, sizeof(char**)*HISTORY_BUFF_SIZE *step);
                if (history->history_commands == NULL)
                    return -1;
                
                step++;
            }
            history->history_commands[history->history_total_commands] = malloc(sizeof(char)*strlen(buffer) + 1);
            if (history->history_commands[history->history_total_commands] == NULL)
                return -1;
            strcpy(history->history_commands[history->history_total_commands], buffer);
            buffer = realloc(buffer, sizeof(char)*HISTORY_BUFF_SIZE);
            buffer_pt = buffer;
            
            history->history_total_commands++;

            tmp_step = step;
            step = 2;
            i = 0;

        }
        else
            buffer_pt[i++] = c;
    }

    if (history->history_total_commands != 0)
    {
        history->history_commands = (char**)realloc(history->history_commands, sizeof(char*)*(history->history_total_commands));
        if (history->history_commands == NULL)
            return -1;

    
        history->current_index = history->history_total_commands;
    }

    free(buffer);
    fclose((*history).history_fd);
    return 0;
}

void destroy_history(SHELL_HISTORY **history)
{
    int i;
    
    if ((*history) != NULL){

    
        if ((*history)->history_commands != NULL){
            for (i = 0; i < (*history)->history_total_commands; i++)
                free((*history)->history_commands[i]);
            free((*history)->history_commands);
        }
        if ((*history)->path != NULL)
            free((*history)->path);
        free((*history));
    }
}


void browse_history_up(SHELL_HISTORY *history)
{
    if (history->current_index != 0)
        history->current_index--;


}
void browse_history_down(SHELL_HISTORY *history)
{
    if (history->current_index < history->history_total_commands)
        history->current_index++;
}
