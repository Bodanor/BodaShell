#ifndef __SHELL_H
#define __SHELL_H
#include <stdio.h>
#include "env.h"
#include "commands.h"

void show_prompt(ENV_t *env);
int init_shell(ENV_t **env);
int shell_execute(char **full_command, ENV_t *env); 
void destroy_shell(ENV_t **env);
#endif
