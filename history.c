#include "history.h"


static void free_loaded_history(SHELL_HISTORY *history)
{
    int i;

    if (history->history_commands != NULL)
    {
        for (i = 0;i < history->history_lines; i++)
            free(history->history_commands[i]);
        free(history->history_commands);
    }
    history->history_lines = 0;
}

int save_command(char *buffer, SHELL_HISTORY *history)
{
    history->hst_descriptor = fopen(history->path, "a");
    if (history->hst_descriptor == NULL)
        return -1;

    fprintf(history->hst_descriptor, "%s\n", buffer);
    fclose(history->hst_descriptor);
    return 1;


}

int init_history(SHELL_HISTORY **history, char *home_dir_path)
{
    (*history) = (SHELL_HISTORY*)malloc(sizeof(SHELL_HISTORY));
    if (*history == NULL)
        return -1;
    
    (*history)->path = (char*)malloc(sizeof(char)*(strlen(home_dir_path) + strlen(HISTORYFILE) + 2));
    if ((*history)->path == NULL)
        return -1;
    
    strcpy((*history)->path, home_dir_path);
    strcat((*history)->path, "/");
    strcat((*history)->path, HISTORYFILE);

    (*history)->history_commands = NULL;
    (*history)->history_lines = 0;
    (*history)->current_index = 0;

    return 1;

}

int load_history(SHELL_HISTORY *history)
{
    int c, step, i, tmp_step;
    char *buffer, *buffer_pt;

    step = 2;
    i = 0;
    buffer = NULL;

    free_loaded_history(history);

    buffer = (char*)malloc(sizeof(char)*HISTORY_BUFF_SIZE);    // First alocate some memory by the ma buffer size
    if (buffer == NULL)
        return -1;

    history->history_commands = (char**)malloc(sizeof(char*)*HISTORY_BUFF_SIZE);
    if (history->history_commands == NULL)
        return -1;

    history->hst_descriptor = fopen(history->path, "r");
    if (history->hst_descriptor == NULL)
        return 0;

    buffer_pt = buffer;
    tmp_step = step;
    

    while((c = fgetc(history->hst_descriptor)) != EOF)
    {
        if ((i + 1) % HISTORY_BUFF_SIZE == 0)
        {
            buffer = (char*) realloc(buffer, sizeof(char*)*HISTORY_BUFF_SIZE *step);
            if (buffer == NULL)
                return -1;
            
            buffer_pt = buffer + (i *(step -1));
            step++;
            i = 0;
        }
        if (c == '\n')
        {
            buffer_pt[i] = '\0';
            
            step = tmp_step;

            if ((history->history_lines + 1) % HISTORY_BUFF_SIZE == 0)
            {
                history->history_commands = (char**)realloc(history->history_commands, sizeof(char**)*HISTORY_BUFF_SIZE *step);
                if (history->history_commands == NULL)
                    return -1;
                
                step++;
            }
            history->history_commands[history->history_lines] = malloc(sizeof(char)*strlen(buffer) + 1);
            if (history->history_commands[history->history_lines] == NULL)
                return -1;
            strcpy(history->history_commands[history->history_lines], buffer);
            buffer = realloc(buffer, sizeof(char)*HISTORY_BUFF_SIZE);
            buffer_pt = buffer;
            
            history->history_lines++;

            tmp_step = step;
            step = 2;
            i = 0;

        }
        else
            buffer_pt[i++] = c;
    }

    if (history->history_lines != 0)
    {
        history->history_commands = (char**)realloc(history->history_commands, sizeof(char*)*(history->history_lines + 1));
        if (history->history_commands == NULL)
            return -1;

    
        history->current_index = history->history_lines;
    }

    free(buffer);
    return 1;
}

void browse_history_up(SHELL_HISTORY *history)
{
    if (history->current_index != 0)
        history->current_index--;


}
void browse_history_down(SHELL_HISTORY *history)
{
    if (history->current_index < history->history_lines )
        history->current_index++;
}