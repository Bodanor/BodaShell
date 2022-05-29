#include <stdio.h>
#include <stdlib.h>

#include "shell.h"

int main(int argc, char **argv)
{
    SHELL_CONF *conf;
    int status;
    int c;
    int i;
    int step;


    char *user_input;
    char *user_pt;

    user_input = NULL;
    conf = NULL;
    status = 1;

    if (init_shell(&conf) != 0)
    {
        printf("%s", colorTypes[1]);
        printf("BSH ERROR : Memory error !\n");
        printf("%s", colorTypes[sizeof(colorTypes) / 8 -1]);
        exit(-1);
    }

    user_input = (char*)malloc(sizeof(SHELL_INPUT_BUFFER));
    user_pt = user_input;
    while(status)
    {
        show_prompt(conf);

        i = 0;
        step = 2;

        while((c = getchar()) != EOF && c != '\n')
        {
            if ((i + 1)% SHELL_INPUT_BUFFER == 0)
            {
                user_input = (char*) realloc(user_input, SHELL_INPUT_BUFFER *step);
                if (user_input == NULL)
                {
                    fprintf(strerror, "%s", colorTypes[1]);
                    fprintf(strerror, "BSH : Memory Error !\n");
                    fprintf(strerror, "%s", colorTypes[sizeof(colorTypes) / 8 -1]);
                    exit(-1);
                }
                user_pt = user_input + (i *(step - 1));
                step++;
                i = 0;
            }
            user_pt[i] = c;
            i++;

        }
        if (user_input != NULL)
        {
            user_pt[i] = '\0';
            user_input = (char*)realloc(user_input, strlen(user_input) + 1);
            if (user_input == NULL)
            {
                fprintf(strerror, "%s", colorTypes[1]);
                fprintf(strerror, "BSH : Memory Error !\n");
                fprintf(strerror, "%s", colorTypes[sizeof(colorTypes) / 8 -1]);
                exit(-1);
            }
        }
        
        if (i != 1)
        {
            //executer commandes
        }
        readShellConf(conf);
    }
    free(user_input);
    
    

}