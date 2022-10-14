#include "utils.h"
#include <stdlib.h>
#include <string.h>

char *trimStr (char *source, char *pattern)
{
    char *tmp_src = source;
    char *tmp_pattern = pattern;
    char *trimmed;

    while (*tmp_src != '\0' && *tmp_pattern != '\0' && *tmp_src == *tmp_pattern){
        tmp_src++;
        tmp_pattern++;
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
