#ifndef __ENV_H__
#define __ENV_H__

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <pwd.h>


typedef struct ENV_CONF_t
{
    char *username;
    uid_t permissions;
} ENV_t;
/**
 * @brief Get the owner shell username and the privileges
 * 
 * @return int : user UID
 */

int get_owner_shell(ENV_t **env);

#endif