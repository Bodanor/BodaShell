#include <stdio.h>
#include <stdlib.h>

#include "shell.h"
#include "env.h"

int main(int argc, char **argv)
{
    char *username = NULL;
    int c;
    int permissions;
    SHELL_CONF *config;

    config = CreateShellConf();
    if (config == NULL)
    {
        perror("BSH : Memory error\n");
    }

    while (1)
    {
        permissions = get_owner_shell(&username);

        readShellConf(config);
        show_prompt(config, username, permissions);
        c = getchar();
    }
    return 0;
}