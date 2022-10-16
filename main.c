#include <stdio.h>
#include <stdlib.h>

#include "commands.h"
#include "shell.h"
#include "history.h"

int main(){
    ENV_t *env ;
    SHELL_HISTORY *history;

    int status;
    char *command;
    char **full_command;

    status = 1;

    if (init_shell(&env) != 0){
        printf("Initialisation failed !\n");
        return -1;
    }

    if (init_history(&history, env->home_dir) != 0){
            perror("Initialisation of history failed !\n");
            return -1;
    }

    if (load_history(history) != 0){
        perror("Could not load history into memory !\n");
    }

    while (status){

        show_prompt(env);
        command = readCommandInput(history, env);
        if (command != NULL){
            full_command = splitCommandInput(command);
            if (*full_command != NULL){
                status = shell_execute(full_command, env);
                free(*full_command);
                free(full_command);
            }
        }

    }
    destroy_history(&history);

}
