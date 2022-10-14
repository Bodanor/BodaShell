#ifndef __COMMANDS_H
#define __COMMANDS_H

#include "env.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_BUFFER_SIZE 512
#define TOK_DELIMITER " \t\r\n\a"

char *readCommandInput(void);
char **splitCommandInput(char *command);
int cd_command(char **full_command, ENV_t *env);
int exit_command(ENV_t *env);
#endif
