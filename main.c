#include <stdio.h>
#include <stdlib.h>

#include "shell.h"

int main(int argc, char **argv)
{
    SHELL_CONF *conf;
    int status;
    char *buffer;
    char **args;

    conf = NULL;
    status = 1;
    
    if (init_shell(&conf) != 0)
    {
        printf("%s", colorTypes[1]);
        printf("BSH ERROR : Memory error !\n");
        printf("%s", colorTypes[sizeof(colorTypes) / 8 -1]);
        exit(-1);
    }

    while(status)
    {

        show_prompt(conf);
        buffer = readCommandInput();
        if (buffer != NULL){
            args = splitCommandInput(buffer);
            shell_launch(args);
        }
        readShellConf(conf);
    }
    free(buffer);
    free_shell(conf);
    
    

}