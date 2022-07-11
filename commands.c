#include "commands.h"

static int display_all_env_variables(ENV_t *env);


int exit_command(char **args)
{
    return 0;
}
int cd_command(char **args, ENV_t **config)
{
    char cwd[256];

    if (args[1] == NULL)
    {
        if (chdir((*config)->home_dir_path) != 0)
        {
            printf("%s", colorTypes[1]);
            printf("BSH ERROR : No such file or directory !\n");
            printf("%s", colorTypes[sizeof(colorTypes) / 8 - 1]);
        }
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            printf("%s", colorTypes[1]);
            printf("BSH ERROR : No such file or directory !\n");
            printf("%s", colorTypes[sizeof(colorTypes) / 8 - 1]);
        }
    }
    if(getcwd(cwd, sizeof(cwd)) == NULL)
    {
        printf("%s", colorTypes[1]);
        printf("BSH ERROR : Environment error !\n");
        printf("%s", colorTypes[sizeof(colorTypes) / 8 - 1]);
    }
    else
    {
        (*config)->curr_path = realloc((*config)->curr_path, sizeof(char) *strlen(cwd) + 1);
        if ((*config)->curr_path == NULL)
            return -1;
        strcpy((*config)->curr_path, cwd);
    }


  return 1;
}

int show_env_command(char **args, ENV_t *env)
{
    if (args[1] != NULL)
    {
        if (strcmp(args[1], "all") == 0)
            return display_all_env_variables(env);
    }

    printf("%s", colorTypes[1]);
    printf("BSH ERROR : Bad syntax !\n\n");
    printf("Possible options :\n");
    printf("<show-env> <all>\t: Show all environment variables.\n");
    printf("<show-env> <something>\t: Show the \"something\" environment variable if existing.\n");
    printf("%s", colorTypes[sizeof(colorTypes) / 8 - 1]);

    return 1;
}

static int display_all_env_variables(ENV_t *env)
{
    int index;
    if (env->envp == NULL)
    {
        printf("%s", colorTypes[1]);
        printf("BSH ERROR : No environment variable found !\n");
        printf("%s", colorTypes[sizeof(colorTypes) / 8 - 1]);
    }
    for (index = 0; env->envp[index]; index ++)
       printf("%s\n", env->envp[index]);
    
    return 1;
}
