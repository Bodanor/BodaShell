#ifndef __SHELL_H__
#define __SHELL_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "env.h"

#define CONFIGFILE "bodashell.conf"

typedef struct SHELL_CONF_t
{
    int root_color;
    int user_color;
    ENV_t *env;

}SHELL_CONF;

int init_shell(SHELL_CONF **conf);
short readShellConf(SHELL_CONF *config);
void show_prompt(SHELL_CONF *config);
#endif