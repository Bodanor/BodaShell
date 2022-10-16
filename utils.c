#include "utils.h"
#include <stdio.h>

char *trimStr (char *source, char *pattern)
{
    char *tmp_src = source;
    char *tmp_pattern = pattern;
    char *trimmed;

    while (*tmp_src != '\0' && *tmp_pattern != '\0' && *tmp_src == *tmp_pattern){
        tmp_src++;
        
    }
    if (*tmp_src != '\0'){
        trimmed = (char*)malloc(sizeof(char)* strlen(tmp_src) + 1);
        if (trimmed == NULL)
            return NULL;
        strcpy(trimmed, tmp_src);
        return trimmed;
    }
    else
        return NULL;

}

int tab_key_action(ENV_t *env)
{
    int i;
    int j;
    struct dirent **namelist;
    extern int errno;
    printf("\n\r");
    if ((i = scandir(env->curr_path, &namelist, 0, alphasort)) != -1){
        for (j = 0; j < i; j++){
            printf("%s\n\r", namelist[j]->d_name);
            free(namelist[j]);
        }
        free(namelist);
    }
    else{
        perror("BSH ERROR ");
        return -1;
    }

    return 0;

}
