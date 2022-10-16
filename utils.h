#ifndef __UTILS_H
#define __UTILS_H

#include "env.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>

char *trimStr(char *source, char *pattern);
int tab_key_action(ENV_t *env);

#endif

