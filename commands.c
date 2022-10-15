#include "commands.h"
#include "env.h"
#include "history.h"
#include "shell.h"
#include <stdio.h>

static int check_special_key(char c, SHELL_HISTORY *history);
static void remove_leftovers(int x_begin, int y_begin);
static void parse_history(SHELL_HISTORY *history, int x_begin, int y_begin);
static void *update_history (SHELL_HISTORY *history); 

static void parse_history(SHELL_HISTORY *history, int x_begin, int y_begin)
{
    remove_leftovers(x_begin, y_begin);
    printf("%s", history->history_commands[history->current_index]);

}
void remove_leftovers(int x_begin, int y_begin)
{
    int x_cur, y_cur;

    get_cursor_pos(&x_cur, &y_cur);

    while (y_begin < y_cur)
    {
        printf("\033[%d;%dH", y_cur,0);
        printf("\033[K");
        y_cur--;
    }

    printf("\033[%d;%dH", y_begin,x_begin);
    printf("\033[K");
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

    char *buffer_tmp;

    history->current_index = history->history_total_commands;
    get_cursor_pos(&x_beginning, &y_beginning);
    i = 0; 
    step = 2;
 
    history->history_commands = (char **)realloc(history->history_commands, sizeof(char*) *(history->history_total_commands + 1));
    if (history->history_commands == NULL){
     perror("History error\n");
     return NULL;
    }

    history->history_commands[history->history_total_commands] = (char*)malloc(sizeof(char)*HISTORY_BUFF_SIZE);
    if (history->history_commands[history->history_total_commands] == NULL){
        printf("Memory Error");
        return NULL;
    }

    *history->history_commands[history->history_total_commands] = '\0';
    while ((c = getchar()) != EOF && c != '\n' && c != '\r'){

        if (check_special_key(c, history))
        {
                parse_history(history, x_beginning, y_beginning);
        }
        else
        {
            if ((i + 1)%HISTORY_BUFF_SIZE == 0){
                buffer_tmp = (char*)realloc(history->history_commands[history->history_total_commands], sizeof(char*)*HISTORY_BUFF_SIZE *step);
                if (history->history_commands[history->history_total_commands] == NULL){
                    printf("Memory Error");
                    return NULL;
                }

                history->history_commands[history->history_total_commands] = buffer_tmp+ (i*(step -1));
                step++;
                i = 0;
            }
            else{
                history->history_commands[history->history_total_commands][i++] = c;
                history->history_commands[history->history_total_commands][i] = '\0';
                putchar(c);
            }

        }
    }


    printf("\n\r");
   
    return update_history(history);
}

void *update_history(SHELL_HISTORY *history)
{
    char *buffer_tmp = history->history_commands[history->history_total_commands];

    if (history->current_index != history->history_total_commands){
        buffer_tmp = realloc(buffer_tmp, sizeof(char) * (strlen(history->history_commands[history->current_index]) + 1));

        if (buffer_tmp == NULL){
            perror("ERREUR DE REALLOC\n");
            return NULL;
        }
        strcpy(buffer_tmp, history->history_commands[history->current_index]);
        history->history_commands[history->history_total_commands] = buffer_tmp;

    }
    else if (*history->history_commands[history->history_total_commands] == '\0'){
        free(history->history_commands[history->history_total_commands]);
        return NULL;
    }
    return history->history_commands[history->history_total_commands++];

}


char **splitCommandInput(char *command)
{
    int position, step;
    char **tokens = NULL;
    char *token = NULL;
    char *tmp_org_command = malloc(sizeof(char)*(strlen(command) + 1));
    if (tmp_org_command == NULL)
        return NULL;

    strcpy(tmp_org_command, command);
    step = 2;
    position = 0;
    tokens = (char**) malloc(sizeof(char*)*INPUT_BUFFER_SIZE);
    
    if (tokens == NULL)
    {
        printf("Error Malloc()\n");
        free(tmp_org_command);
        return NULL;
    }

    token = strtok(tmp_org_command, TOK_DELIMITER);
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
                free(tmp_org_command);
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
