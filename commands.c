#include "commands.h"
#include "env.h"
#include "history.h"
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>

static int check_special_key(char c, SHELL_HISTORY *history);
static void remove_leftovers(int count);
static void parse_history(SHELL_HISTORY *history, int x_begin, int y_begin);
static int update_history (SHELL_HISTORY *history); 

static void parse_history(SHELL_HISTORY *history, int x_begin, int y_begin)
{
    int x_current, y_current;
    get_cursor_pos(&x_current, &y_current);

    remove_leftovers(x_current - x_begin);
        printf("%s", history->history_commands[history->current_index]);

}
void remove_leftovers(int count)
{
    int x;

    for (x = count; x!= 0; x--)
        printf("\b \b");
}
int check_special_key(char c, SHELL_HISTORY *history)
{
    if (c == '\033'){

        /* Rearrange this code as the return statement is redundant */ 
        getchar();
        switch(getchar())
        {
            case 'A':
                browse_history_up(history);
                return 1;
            case 'B':
                browse_history_down(history);
                return 1;
            case 'C':
                return 1;
                break;
            case 'D':
                return 1;
                break;
        }
    }
    return 0;
}


char *readCommandInput(SHELL_HISTORY *history)
{
    int i, step, c;
    int y_beginning, x_beginning;
    char *buffer;
    char *buffer_pt;

    history->current_index = history->history_total_commands;
    get_cursor_pos(&x_beginning, &y_beginning);
    i = 0; 
    step = 2;

    buffer = (char*)malloc(sizeof(char)*HISTORY_BUFF_SIZE);
    if (buffer == NULL){
        printf("Memory Error");
        return NULL;
    }

    buffer_pt = buffer;
    *buffer_pt = '\0';
    history->history_commands[history->history_total_commands] = buffer_pt;

    while ((c = getchar()) != EOF && c != '\n' && c != '\r'){

        if (check_special_key(c, history))
        {
                parse_history(history, x_beginning, y_beginning);
        }
        else
        {
            if ((i + 1)%HISTORY_BUFF_SIZE == 0){
                buffer = (char*)realloc(buffer, sizeof(char*)*HISTORY_BUFF_SIZE *step);
                if (buffer == NULL){
                    printf("Memory Error");
                    return NULL;
                }
                buffer_pt = buffer+ (i*(step -1));
                step++;
                i = 0;
            }
            else{
                buffer_pt[i++] = c;
                buffer_pt[i] = '\0';
                putchar(c);
            }

        }
    }


    printf("\n\r");

    if (history->current_index != history->history_total_commands){
        buffer = realloc(buffer, sizeof(char) * (strlen(history->history_commands[history->current_index]) + 1));
        if (buffer == NULL){
            perror("ERREUR DE REALLOC\n");
            return NULL;
        }
        strcpy(buffer, history->history_commands[history->current_index]);
    }
    if (*buffer == '\0'){
        free(buffer);
        return NULL;
    }
    else{

        history->history_commands[history->history_total_commands] = buffer;
        return history->history_commands[history->history_total_commands++];
    }
}



char **splitCommandInput(char *command)
{
    int position, step;
    char **tokens = NULL;
    char *token = NULL;

    step = 2;
    position = 0;
    tokens = (char**) malloc(sizeof(char*)*INPUT_BUFFER_SIZE);
    
    if (tokens == NULL)
    {
        printf("Error Malloc()\n");
        return NULL;
    }

    token = strtok(command, TOK_DELIMITER);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if ((position+ 1) % INPUT_BUFFER_SIZE == 0)
        {
            tokens = realloc(tokens, sizeof(char*)*INPUT_BUFFER_SIZE*step);
            if (tokens == NULL)
            {
                printf("Error Alloc()\n");
                return NULL;
            }
        }
        token = strtok(NULL, TOK_DELIMITER);
    }
    tokens[position] = NULL;
    return tokens;
}

int cd_command(char **full_command, ENV_t *env)
{
    char cwd[256];

    if (full_command[1] == NULL){
        if (chdir(env->home_dir) != 0){
            printf("Failed to change to user home directory !\n");
        }
    }
    else
        if (chdir(full_command[1])!= 0)
                perror("SH : ");

    if (getcwd(cwd, sizeof(cwd)) == NULL)
        perror("SH : ");

    else{
        env->curr_path = realloc(env->curr_path, sizeof(char)*strlen(cwd) + 1);
        if (env->curr_path == NULL){
            printf("Error : Malloc()");
            return -1;
        }
        strcpy(env->curr_path, cwd);
    }
    return 1;
}


int exit_command(ENV_t *env)
{
    switch_to_non_canonical(env);
    destroy_shell(&env);
    return 0;
}
