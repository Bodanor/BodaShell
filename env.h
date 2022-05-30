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
 * @brief Get the shell owner informations
 * 
 * @param env double pointer to a ENV_t struct
 * 
 * @post: Allocated ENV_t structure containing user infos
 * @return:
 *         0 : Ff successfull
 *         -1: If memory error  
 */
int get_owner_shell(ENV_t **env);

#endif