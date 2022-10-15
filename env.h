#ifndef __ENV_H
#define __ENV_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <termio.h>

typedef struct ENV_CONF_t
{
    char *username;
    uid_t permissions;
    char *home_dir;
    char *curr_path;
    char*hostname;
    struct termios oldterm;
    struct termios newterm;

} ENV_t;

int get_shell_owner(ENV_t **env);

#endif
