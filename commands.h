#ifndef __COMMANDS__H
#define __COMMANDS__H

#include <stdio.h>
#include <stdlib.h>

#include "env.h"
#include "shell.h"

int cd_command(char **args, ENV_t **config);
int exit_command(char **args);


#endif