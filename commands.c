#include "commands.h"

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