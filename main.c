#include <stdio.h>
#include <stdlib.h>

#include "shell.h"

int main(int argc, char **argv)
{
    SHELL_CONF *conf;

    conf = NULL;

    if (init_shell(&conf) != 0)
    {
        printf("BSH : Memory error !\n");
        exit(-1);
    }

    while(1)
    {
        show_prompt(conf);
        getchar();
    }
    
    

}