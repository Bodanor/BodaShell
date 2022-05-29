#include "shell.h"


static const char *colorName[] = {
    "black",
    "red",
    "green",
    "yellow",
    "blue",
    "purple",
    "cyan",
    "white",
};

static const char *colorTypes[] = {
    "\033[0;30m",   //Black
    "\033[0;31m",   //Red
    "\033[0;32m",   //Green
    "\033[0;33m",   //Yellow
    "\033[0;34m",   //Blue
    "\033[0;35m",   //Purple
    "\033[0;36m",   //Cyan
    "\033[0;37m",   //White

};


struct SHELL_CONF_t
{
    int root_color;
    int user_color;

};

SHELL_CONF *CreateShellConf(void)
{
    SHELL_CONF *conf;
    conf = (SHELL_CONF*)malloc(sizeof(SHELL_CONF));
    
    return conf;
}


short readShellConf(SHELL_CONF *config)
{
    assert(config != NULL);

    FILE *fp;
    int c;
    int i;
    char buffer[50];

    config->root_color = 1;
    config->user_color = 2;


    fp = fopen("bodashell.conf", "r");
    if (fp == NULL)
    {
        fp = fopen("bodashell", "w");
        return -1;
    }
    else
    {
        i = 0;
        do
        {
            c = fgetc(fp);

            if (c == '=')
            {
                buffer[i] = '\0';
                if (strcmp(buffer, "root_color") == 0)
                {
                    i = 0;

                    while ((c = fgetc(fp)) != '\n')
                    {
                        buffer[i++] = c;
                    }
                    buffer[i] = '\0';

                    for (i = 0; i < sizeof(colorName) / 8; i++)
                    {
                        if (strcmp(buffer, colorName[i]) == 0)
                        {
                            config->root_color = i;
                        }
                    }

                    i = 0;
                }
                else if (strcmp(buffer, "user_color") == 0)
                {
                    i = 0;

                    while ((c = fgetc(fp)) != '\n' && c != EOF)
                    {
                        buffer[i++] = c;
                    }
                    buffer[i] = '\0';

                    for (i = 0; i < sizeof(colorName) / 8; i++)
                    {
                        if (strcmp(buffer, colorName[i]) == 0)
                        {
                            config->user_color = i;
                        }
                    }

                    i = 0;
                }
                else
                {
                    printf("Bad parameter \"%s\" in config file !\n", buffer);
                }
            }
            else
            {
                if (i < 49)
                    buffer[i] = c;
                i++;
            }

        }while (c != EOF);
    }

}

void show_prompt(SHELL_CONF *config, char *username, int permissions)
{
    assert(config != NULL && username != NULL && permissions != -1);

    if (permissions != 0)
        printf("%s", colorTypes[config->user_color]);
    else
        printf("%s", colorTypes[config->root_color]);
    
    printf("%s #~", username);
    printf("\033[0m");
}

