#include "shell.h"
#include "env.h"
#include "utils.h"
#include <stdio.h>

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
