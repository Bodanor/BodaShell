#include <stdio.h>
#include <stdlib.h>

#include "shell.h"

int main(int argc, char **argv)
{
    SHELL_CONF *conf;

    conf = NULL;

    if (init_shell(&conf) != 0)
    {
        printf("%s", colorTypes[1]);
        printf("BSH ERROR : Memory error !\n");
        printf("%s", colorTypes[sizeof(colorTypes) / 8 -1]);
        exit(-1);
    }

    while(1)
    {
        show_prompt(conf);
        getchar();
        readShellConf(conf);
    }
    
    

}