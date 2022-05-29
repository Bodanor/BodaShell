#include "env.h"

int get_owner_shell(char **username)
{
    struct passwd *pwd;
    uid_t permissions;

    permissions = getuid();
    pwd = getpwuid(permissions);

    if (*username != NULL)
        free(*username);
        
    *username = (char*)malloc(sizeof(char) *strlen(pwd->pw_name) + 1);
    if (*username == NULL)
        return -1;
    else
    {
        strcat(*username, pwd->pw_name);
        return permissions;
    }
}