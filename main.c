#include <stdio.h>
#include <stdlib.h>

#include "commands.h"
#include "shell.h"
int main()
{
    ENV_t *env ;
    int status;
    char *command;
    char **full_command;

    status = 1;

    if (init_shell(&env) != 0){
        printf("Initialisation failed !\n");
        return -1;
    }

    while (status){

        show_prompt(env);
        command = readCommandInput();
        if (command != NULL){
            full_command = splitCommandInput(command);
            status = shell_execute(full_command, env);
            free(command);
            free(full_command);
        }

    }



}
