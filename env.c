#include "env.h"

int get_shell_owner(ENV_t **env)
{
    /*
     * Don't need to free this below (pwd) as it is initilized by getpuid function using "static", so it's on the data segment not in the stack.
     * It reamins on the scope till the program lives
    */
    struct passwd *pwd;
    char cwd[256];
    char hst[256];

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
    (*env)->home_dir_path = (char*)malloc(sizeof(char)*strlen(pwd->pw_dir) + 1);
    if(getcwd(cwd, sizeof(cwd)) == NULL)
    {
        return -1;
    }
    (*env)->curr_path = (char*)malloc(sizeof(char)*strlen(cwd) + 1);

    if (gethostname(hst, sizeof(hst)) == -1)
    {
        return -1;
    }
    (*env)->hostname = (char*)malloc(sizeof(char)*strlen(hst) + 1);

    strcpy((*env)->hostname, hst);
    
    if ((*env)->username == NULL || (*env)->home_dir_path == NULL || (*env)->curr_path == NULL)
        return -1;
    
    else
    {
        strcpy((*env)->username, pwd->pw_name);
        strcpy((*env)->home_dir_path, pwd->pw_dir);
        strcpy((*env)->curr_path, cwd);
    }

    return 0;
}