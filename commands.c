#include "commands.h"
#include "env.h"
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>

char *readCommandInput(void)
{
    int i, step, c;
    char *buffer;
    char *buffer_pt;

    buffer = NULL;
    i = 0; 
    step = 2;

    buffer = (char*)malloc(sizeof(char)*INPUT_BUFFER_SIZE);
    if (buffer == NULL){
        printf("Memory Error");
        return NULL;
    }

    buffer_pt = buffer;

    while ((c = getchar()) != EOF && c != '\n'){
        if ((i + 1)%INPUT_BUFFER_SIZE == 0){
            
            buffer = (char*)realloc(buffer, sizeof(char*)*INPUT_BUFFER_SIZE);
            if (buffer == NULL){
                printf("Memory Error");
                return NULL;
            }
            buffer_pt = buffer + (i*(step -1));
            step++;
            i++;
        }
        buffer_pt[i++] = c;
    }

    if (i != 0){
        buffer_pt[i] = '\0';

        buffer = (char*)realloc(buffer, strlen(buffer) + 1);
        if (buffer == NULL){
            printf("Memory Error");
            return NULL;
        }
        return buffer;
    }
    else{
        free(buffer);
        return NULL;
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
    destroy_shell(&env);
    return 0;
}
