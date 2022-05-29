#ifndef __SHELL_H__
#define __SHELL_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "env.h"

#define CONFIGFILE "bodashell.conf"

static const char *colorName[] = {
    "black",
    "red",
    "green",
    "yellow",
    "blue",
    "purple",
    "cyan",
    "white",
};

static const char *colorTypes[] = {
    "\033[1;30m",   //Black
    "\033[1;31m",   //Red
    "\033[1;32m",   //Green
    "\033[1;33m",   //Yellow
    "\033[1;34m",   //Blue
    "\033[1;35m",   //Purple
    "\033[1;36m",   //Cyan
    "\033[1;37m",   //White
    "\033[0m",      //RESET

};


typedef struct SHELL_CONF_t
{
    int root_color;
    int user_color;
    short warning_flag;
    ENV_t *env;


}SHELL_CONF;

int init_shell(SHELL_CONF **conf);
short readShellConf(SHELL_CONF *config);
void show_prompt(SHELL_CONF *config);
#endif