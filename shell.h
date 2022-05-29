#ifndef __SHELL_H__
#define __SHELL_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct SHELL_CONF_t SHELL_CONF;


SHELL_CONF *CreateShellConf(void);
short readShellConf(SHELL_CONF *config);
void show_prompt(SHELL_CONF *config, char *username, int permissions);
#endif