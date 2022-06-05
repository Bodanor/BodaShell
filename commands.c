#include "commands.h"

int exit_command(char **args)
{
    return 0;
}
int cd_command(char **args, ENV_t **config)
{
    if (args[1] == NULL)
    {
        if (chdir((*config)->home_dir_path) != 0)
        {
            printf("%s", colorTypes[1]);
            printf("BSH ERROR : No such file or directory !\n");
            printf("%s", colorTypes[sizeof(colorTypes) / 8 - 1]);
        }
        else
            strcpy((*config)->curr_path, (*config)->home_dir_path);
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            printf("%s", colorTypes[1]);
            printf("BSH ERROR : No such file or directory !\n");
            printf("%s", colorTypes[sizeof(colorTypes) / 8 - 1]);
        }
        else
            strcpy((*config)->curr_path, args[1]);
    }

  return 1;
}