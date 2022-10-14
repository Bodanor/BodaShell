#ifndef __ENV_H
#define __ENV_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>

typedef struct ENV_CONF_t
{
    char *username;
    uid_t permissions;
    char *home_dir;
    char *curr_path;
    char*hostname;
} ENV_t;

int get_shell_owner(ENV_t **env);

#endif
