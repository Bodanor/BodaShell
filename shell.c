#include "shell.h"
#include "env.h"
#include "utils.h"
#include <stdio.h>
#include <unistd.h>

void switch_to_canonical(ENV_t *env)
{
    tcgetattr(STDIN_FILENO, &env->oldterm);
    env->newterm = env->oldterm;
   
    cfmakeraw(&env->newterm);
    tcsetattr(STDIN_FILENO, TCSANOW, &env->newterm);

}

void switch_to_non_canonical(ENV_t *env)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &env->oldterm);
}

void get_cursor_pos(int *x, int *y)
{
    *x = *y = -1;
    printf("\033[6n");
    fflush(stdout);
    scanf("\033[%d;%dR", y, x);
}

void show_prompt(ENV_t *env)
{
    char *trimmed_homedir = NULL;

    printf("%s", env->username);
    putchar('@');
    printf("%s", env->hostname);
    putchar(':');
    if (strcmp(env->curr_path, env->home_dir) == 0){
        putchar('~');
    }

    else if (strstr(env->curr_path, env->home_dir) != NULL){
        putchar('~');
        if ((trimmed_homedir = trimStr(env->curr_path, env->home_dir)) == NULL)
            printf("SH : Trim Error\n");
        else 
            printf("%s", trimmed_homedir);
    }
    else
        printf("%s", env->curr_path);
    
    printf(" $ ");

    free(trimmed_homedir);
}

int init_shell(ENV_t **env)
{
    if (get_shell_owner(env) != 0)
        return -1;
    switch_to_canonical(*env);
    return 0;

}

int shell_execute(char **full_command, ENV_t *env)
{
    if (strcmp(full_command[0], "cd") == 0)
        return (cd_command(full_command, env));
    else if (strcmp(full_command[0], "exit") == 0)
        return exit_command(env);

    return 1;
}

void destroy_shell(ENV_t **env)
{
    if ((*env)->curr_path != NULL)
        free((*env)->curr_path);

    if ((*env)->home_dir != NULL)
        free((*env)->home_dir);

    if ((*env)->hostname != NULL)
        free((*env)->hostname);

    if ((*env)->username != NULL)
        free((*env)->username);

    if (*env != NULL)
        free(*env);
}
