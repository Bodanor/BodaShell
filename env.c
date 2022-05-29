#include "env.h"

int get_owner_shell(ENV_t **env)
{
    struct passwd *pwd;

    if (*env != NULL)
    {
        free((*env)->username);
        free(*env);
    }   

    *env = (ENV_t*)malloc(sizeof(ENV_t));
    if (env == NULL)
        return -1;
    

    (*env)->permissions = getuid();
    pwd = getpwuid((*env)->permissions);
        
    (*env)->username = (char*)malloc(sizeof(char) *strlen(pwd->pw_name) + 1);
    if ((*env)->username == NULL)
        return -1;
    else
    {
        strcpy((*env)->username, pwd->pw_name);
        return 0;
    }
}