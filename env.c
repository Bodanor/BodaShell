#include "env.h"
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int get_shell_owner(ENV_t **env)
{
    struct passwd *pwd;
    char cwd[256];
    char hostname[256];

    *env = (ENV_t*)malloc(sizeof(ENV_t));
    if (env == NULL)
        return -1;

    (*env)->permissions = getuid();
    pwd = getpwuid((*env)->permissions);

    (*env)->username = (char*)malloc(sizeof(char)*strlen(pwd->pw_name) + 1);
    (*env)->home_dir = (char*)malloc(sizeof(char)*strlen(pwd->pw_dir) + 1);
    
    if (getcwd(cwd, sizeof(cwd)) == NULL){
        perror("Error retrieving current working directory !");
        return -1;
    }
    (*env)->curr_path = (char*)malloc(sizeof(char)*strlen(cwd) + 1);

    if (gethostname(hostname, sizeof(hostname)) == -1)
    {
        perror("Error retrieving current hostname !");
        return -1;
    }
    (*env)->hostname = (char*)malloc(sizeof(char)*strlen(hostname) + 1);

    if ((*env)->username == NULL || (*env)->home_dir == NULL || (*env)->curr_path == NULL || (*env)->hostname == NULL){
        perror("Malloc error ");
        return -1;
    }
    else{
        strcpy((*env)->username, pwd->pw_name);
        strcpy((*env)->home_dir, pwd->pw_dir);
        strcpy((*env)->curr_path, cwd);
        strcpy((*env)->hostname, hostname);
    }
    return 0;

}
