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
    setvbuf(stdout, NULL, _IONBF, 0);

}

void switch_to_non_canonical(ENV_t *env)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &env->oldterm);
}

void get_cursor_pos(int *x, int *y)
{
    char buf[30] = {0};
    int ret, i, pow;
    char ch;

    *y = 0;
    *x = 0;

    struct termios term, restore;

    tcgetattr(STDIN_FILENO, &term);
    tcgetattr(STDIN_FILENO, &restore);
    term.c_cflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    write (1, "\033[6n", 4);

    for (i = 0, ch = 0; ch != 'R'; i++)
    {
        ret = read(0, &ch, 1);
        if (!ret){
            tcsetattr(STDIN_FILENO, TCSANOW, &restore);
            printf("Error");
        }
        buf[i] = ch;
    }

    if (i < 2){
        tcsetattr(STDIN_FILENO, TCSANOW, &restore);
        printf("Error");
    }

    for (i -= 2, pow = 1; buf[i] != ';'; i--, pow *=10)
        *x = *x + (buf[i] - '0') *pow;
    for( i-- , pow = 1; buf[i] != '['; i--, pow *= 10)
        *y = *y + ( buf[i] - '0' ) * pow;

 
    tcsetattr(STDIN_FILENO, TCSANOW, &restore);
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
