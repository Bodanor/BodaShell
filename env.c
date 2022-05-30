#include "env.h"

int get_owner_shell(ENV_t **env)
{
    /*
     * Don't need to free this below (pwd) as it is initilized by getpuid function using "static", so it's on the data segment not in the stack.
     * It reamins on the scope till the program lives
    */
    struct passwd *pwd;

    if (*env != NULL)
    {
        free((*env)->username);
        free(*env);
    }   

    *env = (ENV_t*)malloc(sizeof(ENV_t));
    if (env == NULL)
        return -1;
    

    (*env)->permissions = getuid(); // Get the user UID.
    pwd = getpwuid((*env)->permissions);    // From the UID get informations from the specific user with this UID.
        
    (*env)->username = (char*)malloc(sizeof(char) *strlen(pwd->pw_name) + 1);
    if ((*env)->username == NULL)
        return -1;
    else
    {
        strcpy((*env)->username, pwd->pw_name);
        return 0;
    }
}