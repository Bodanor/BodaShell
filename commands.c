#include "commands.h"

int exit_command(char **args)
{
    return 0;
}
int cd_command(char **args, ENV_t **config)
{
    int status;

    if (args[1] == NULL)
        status = chdir((*config)->home_dir_path);
    else
         status = chdir(args[1]);
    
   if (status != 0)
   {
        printf("%s", colorTypes[1]);
        printf("BSH ERROR : No such file or directory !\n");
        printf("%s", colorTypes[sizeof(colorTypes) / 8 - 1]);
   }

  return 1;
}