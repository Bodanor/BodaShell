#include "shell.h"

static short parse_user_color(SHELL_CONF *conf, FILE *fp, char key_buffer[50]);
static short parse_root_color(SHELL_CONF *conf, FILE *fp, char key_buffer[50]);


static short parse_user_color(SHELL_CONF *conf, FILE *fp, char key_buffer[50])
{
    char value_buffer[50];
    int i = 0;
    int c;
    short flag = 0; // know if a valid value has been found for the "user_color" key.

    while ((c = fgetc(fp)) != '\n' && c != EOF)
    {
        if (i < 49)
            value_buffer[i++] = c;
    }

    value_buffer[i] = 0;

    for (i = 0; i < sizeof(colorName) / 8; i++)
    {
        if (strcmp(value_buffer, colorName[i]) == 0)
        {
            conf->user_color = i;
            flag = 1;
        }
    }

    if (flag == 0)  // If no value has been found then we warn the user that default settings are applied.
    {
        if (conf->warning_flag == 0)
        {
            printf("%s", colorTypes[3]);
            printf("BSH WARNING : Unknown value \"%s\" with parameter \"%s\" ! Defaulting to red\n", value_buffer, key_buffer);
            printf("%s", colorTypes[sizeof(colorTypes) / 8 - 1]);
        }
        return -1;
    }
    return 0;

};

static short parse_root_color(SHELL_CONF *conf, FILE *fp, char key_buffer[50])
{
    char value_buffer[50];
    int i = 0;
    int c;
    short flag = 0; // know if a valid value has been found for the "root_color" key.

    while ((c = fgetc(fp)) != '\n' && c != EOF)
    {
        if (i < 49)
            value_buffer[i++] = c;
    }

    value_buffer[i] = 0;

    for (i = 0; i < sizeof(colorName) / 8; i++)
    {
        if (strcmp(value_buffer, colorName[i]) == 0)
        {
            conf->root_color = i;
            flag = 1;
        }
    }

    if (flag == 0)  // If no value has been found then we warn the user that default settings are applied.
    {
        if (conf->warning_flag == 0)
        {
            printf("%s", colorTypes[3]);
            printf("BSH WARNING : Unknown value \"%s\" with parameter \"%s\" ! Defaulting to red\n", value_buffer, key_buffer);
            printf("%s", colorTypes[sizeof(colorTypes) / 8 - 1]);
        }
        return -1;
    }
    return 0;

};


int init_shell(SHELL_CONF **conf)
{
    short status = 0;

    *conf = (SHELL_CONF*)malloc(sizeof(SHELL_CONF));
    if (*conf == NULL)
        return -1;
    
    (*conf)->env = NULL;

    if (get_owner_shell(&((*conf)->env)) != 0)  // Know who launched the shell and his permissions.
        return -1;
    
    (*conf)->warning_flag = 0;  // Set the warning flag to zero to allow at least one time to warn user for config errors.
    
    status = readShellConf(*conf);
    if (status == -1)  // Read the config file.
    {
        printf("%s", colorTypes[3]);
        printf("BSH WARNING : No config file found. Default has been created\n");
        printf("%s", colorTypes[sizeof(colorTypes) / 8 - 1]);

    }
    else if (status == -2)
    {
        printf("%s", colorTypes[1]);
        printf("BSH ERROR : Could not create a config file. Check your permissions !\n");
        printf("%s", colorTypes[sizeof(colorTypes) / 8 - 1]);
    }
    return 0;
    
}


short readShellConf(SHELL_CONF *config)
{
    assert(config != NULL);

    FILE *fp;
    int c;
    int i;
    char key_buffer[50];

    // Flags to control if we have already processed some parameters and avoid overwritting them.
    // We warn the user of redundancy in the config file.
    int flags[3] = {0, 0};

    fp = fopen(CONFIGFILE, "r");
    if (fp == NULL)
    {
        config->root_color = 1;
        config->user_color = 2;

        fp = fopen(CONFIGFILE, "wr");
        if (fp == NULL)
            return -2;
        else
        {
            fprintf(fp, "root_color=red\n");
            fprintf(fp, "user_color=green");
            fclose(fp);
            return -1;
        }
    }
    i = 0;
    while ((c = fgetc(fp)) != EOF)
    {
        if (c == '#' || c == ';')   // Ignore commented lines in the config file.
            while ((c = fgetc(fp)) != '\n' && c != EOF)
                ;

        else if (c == '=')
        {
            key_buffer[i] = '\0';

            /*
                * Check every possible parameters that are in the config file and process them 
                * We also check every flag to see the config doesn't contains multiple times the same parameter
                */
        
            if (flags[0] == 0)
            {
                if (strcmp(key_buffer, "user_color") == 0)
                {
                    if (parse_user_color(config, fp, key_buffer) == -1)
                        config->user_color = 2;

                    flags[1] = 1;
                    i = 0;
                }
                else if (strcmp(key_buffer, "root_color") == 0)
                {
                    if (parse_root_color(config, fp, key_buffer) == -1)
                        config->user_color = 1;
                
                    flags[0] = 1;
                    i = 0;
                }
            }
            else    // If we have already processed a specific parameter, we end up here and warn the user of the redundancy.
            {
                if (config->warning_flag == 0)
                {
                    printf("%s", colorTypes[3]);
                    printf("BSH WARNING : Redefinition of parameter \"%s\"\n", key_buffer);
                    printf("%s", colorTypes[sizeof(colorTypes) / 8 - 1]);
                }
                while ((c = fgetc(fp)) != '\n' && c != EOF)
                    ;
                i = 0;
            }
        }
        else
        {
            if (i < 49)
                key_buffer[i++] = c;
        }
    }

    config->warning_flag = 1;   // If user has already been warned, set this flag to disable warnings every time we read config file again.
    fclose(fp);

    return 0;

}

void show_prompt(SHELL_CONF *config)
{
    assert(config != NULL && config->env != NULL && config->env->username != NULL && config->env->permissions >= 0);

    if (config->env->permissions != 0)  // If user is non-root we set the proper color output
        printf("%s", colorTypes[config->user_color]);   
    else
        printf("%s", colorTypes[config->root_color]);   // If user is root we set the proper color output
    
    printf("%s ~$ ", config->env->username);
    printf("%s", colorTypes[sizeof(colorTypes) / 8 - 1]);   // Reset the color.
}


char *readCommandInput(void)
{
    int i, step, c;
    char *buffer;
    char *buffer_pt;

    buffer = NULL;
    i = 0;
    step = 2;

    buffer = (char*)malloc(sizeof(char)*SHELL_INPUT_BUFFER);    // First alocate some memory by the ma buffer size
    if (buffer == NULL)
    {
        fprintf(stderr, "%s", colorTypes[1]);
        fprintf(stderr, "BSH : Memory Error !\n");
        fprintf(stderr, "%s", colorTypes[sizeof(colorTypes) / 8 -1]);
        return NULL;
    }

    buffer_pt = buffer;

    while((c = getchar()) != EOF && c != '\n')  // Get input char by char till user presses Enter
    {
        if ((i + 1)% SHELL_INPUT_BUFFER == 0)   // if buffer is full, we reallocate it by step*buffer_size.
        {
            buffer = (char*) realloc(buffer, sizeof(char*)*SHELL_INPUT_BUFFER *step);
            if (buffer == NULL)
            {
                fprintf(stderr, "%s", colorTypes[1]);
                fprintf(stderr, "BSH : Memory Error !\n");
                fprintf(stderr, "%s", colorTypes[sizeof(colorTypes) / 8 -1]);
                return NULL;
            }

            /*
            * avoid having buffer_pt pointing to the beginning of buffer.
            * We continue where we left by pointing it at the end of buffer and continue to read the input
            */

            buffer_pt = buffer + (i *(step - 1));
            step++; //increment the step for the next realloc
            i = 0;
        }
        buffer_pt[i++] = c;

    }
    if (i != 0) // We the input is not empty.
    {
        buffer_pt[i] = '\0';    // DON'T FORGET TO PUT A NULL CHAR AT THE END OF THE BUFFER !!

        /*
         * Since we increment the buffer by step*buffer_size every time it is full , there may be some memory wasting.
         * Readjust the whole buffer to the right size of input. Don't waste memory !
         * Let's say input is 513 chars, buffer 512 and we reallocate 512 to buffer, we will waste 511 bytes !
         */

        buffer = (char*)realloc(buffer, strlen(buffer) + 1);
        if (buffer == NULL)
        {
            fprintf(stderr, "%s", colorTypes[1]);
            fprintf(stderr, "BSH : Memory Error !\n");
            fprintf(stderr, "%s", colorTypes[sizeof(colorTypes) / 8 -1]);
            return NULL;
        }
        return buffer;
    }
    else
    {
        free(buffer);   // If input is empty, free it for the next call.
        return NULL;
    }
}

void free_shell(SHELL_CONF *conf)
{
    assert(conf != NULL);
    free(conf->env->username);
    free(conf->env);
    free(conf);
}