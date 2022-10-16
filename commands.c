#include "commands.h"
#include "env.h"
#include "history.h"
#include "shell.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int check_arrow_key(SHELL_HISTORY *history);
static void remove_leftovers(int x_begin, int y_begin);
static int parse_history(SHELL_HISTORY *history, char **orig_buffer, int x_begin, int y_begin);
static void *update_history (SHELL_HISTORY *history); 
static void delete_char(SHELL_HISTORY *history, int x_begin , int y_begin, int *i);
static int process_char(char c, SHELL_HISTORY *history, char *buffer_tmp, int *step, int *i);

int process_char(char c, SHELL_HISTORY *history, char *buffer_tmp, int *step, int *i)
{
    /* If we press the up array and try to edit the line, we don't to a realloc perhaps checking variable i against srlen would
     * be a solution. Anyway, every time we press up arrow and edit, we write outside of the array boundery */

    if ((*i + 1)%HISTORY_BUFF_SIZE == 0){
        buffer_tmp = (char*)realloc(history->history_commands[history->history_total_commands], sizeof(char*)*HISTORY_BUFF_SIZE * *step);
        if (history->history_commands[history->history_total_commands] == NULL){
            printf("Memory Error");
            return -1;
        }
        /*This is wrong, buffer_tmp is going to point somewhere else and thus cant be free anymore. Also pass a pointer to buffer in case it changes */
        history->history_commands[history->history_total_commands] = buffer_tmp+ (*i*(*step -1));
        (*step)++;
        *i = 0;
    }
    else{
        history->history_commands[history->history_total_commands][(*i)++] = c;
        history->history_commands[history->history_total_commands][*i] = '\0';
        putchar(c);
    }

    return 0;

}

static int parse_history(SHELL_HISTORY *history, char **orig_buffer, int x_begin, int y_begin)
{
    char *buffer_tmp = history->history_commands[history->history_total_commands];

    if (history->current_index != history->history_total_commands){
        if (*orig_buffer == NULL){
            *orig_buffer = (char*)malloc(sizeof(char)*(strlen(buffer_tmp) + 1));
            if (orig_buffer == NULL){
                perror("BSH ERROR ");
                return -1;
            }
            strcpy(*orig_buffer, buffer_tmp);
        }

        buffer_tmp = (char*)realloc(buffer_tmp, sizeof(char)*(strlen(history->history_commands[history->current_index]) + 1));
        if (buffer_tmp == NULL){
            perror("BSH ");
            return -1;
        }
            
        strcpy(buffer_tmp, history->history_commands[history->current_index]);
        history->history_commands[history->history_total_commands] = buffer_tmp;
    }
    
    else{
        if (*orig_buffer != NULL){
            buffer_tmp = (char*)realloc(buffer_tmp, sizeof(char) *(strlen(*orig_buffer) + 1));
            if (buffer_tmp == NULL){
                perror("BSH ERROR ");
                return -1;
            }
            strcpy(buffer_tmp, *orig_buffer);
            history->history_commands[history->history_total_commands] = buffer_tmp;
            free(*orig_buffer);
            *orig_buffer = NULL;
        }

    }
    remove_leftovers(x_begin, y_begin);
    printf("%s", history->history_commands[history->history_total_commands]);

    return 0;
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
int check_arrow_key(SHELL_HISTORY *history)
{
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
    return 0;
}


char *readCommandInput(SHELL_HISTORY *history, ENV_t *env)
{
    int i, step, c;
    int y_beginning, x_beginning;

    char *buffer_tmp = NULL;
    char *orig_buffer = NULL;

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

        switch(c){
            case '\033': 
                if (check_arrow_key(history)){
                    parse_history(history, &orig_buffer, x_beginning, y_beginning);
                    i = strlen(history->history_commands[history->history_total_commands]);
                }
                break;

            case 127:
                delete_char(history, x_beginning, y_beginning, &i);
                break;

            case 9:
                /* If we get here and dont send a tab key again, we lose one keystroke. This is a small issue
                 * I need to the the while getchar to consume this lost keystroke
                 * */

                if (getchar() == 9){
                    tab_key_action(env);
                    show_prompt(env);
                    get_cursor_pos(&x_beginning, &y_beginning);
                    printf("%s", history->history_commands[history->current_index]);

                }
                break;

            default:
                if (!iscntrl(c))
                    if (process_char(c, history, buffer_tmp, &step, &i) == -1)
                        return NULL;
                break;
        }
        
    }
    if (orig_buffer != NULL)
        free(orig_buffer);

    printf("\n\r");
   
    return update_history(history);
}

void delete_char(SHELL_HISTORY *history, int x_begin , int y_begin, int *i)
{
    int x_current, y_current;
    struct winsize w;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    get_cursor_pos(&x_current, &y_current);

    if (x_current == 1 && y_current != y_begin){
        printf("\033[%d;%dH", y_current -1, w.ws_col);
        printf("\033[K");
    }
    else if (x_current != x_begin || y_current != y_begin)
    {
        printf("\033[%d;%dH", y_current,x_current -1);
        printf("\033[K");
        history->history_commands[history->history_total_commands][--(*i)] = '\0';

    }

}

void *update_history(SHELL_HISTORY *history)
{
    if (*history->history_commands[history->history_total_commands] == '\0'){
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
            printf("Failed to change to user home directory !\n\r");
        }
    }
    else
        if (chdir(full_command[1])!= 0){
            fprintf(stderr, "BSH : %s\n\r", strerror(errno));
        }

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
