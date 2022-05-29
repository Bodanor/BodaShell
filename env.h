#ifndef __ENV_H__
#define __ENV_H__

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <pwd.h>



/**
 * @brief Get the owner shell username and the privileges
 * 
 * @return int : user UID
 */

int get_owner_shell(char **username);

#endif