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

static const char *possible_options[] = {
    "root_color",
    "user_color",
    "display_path",
    "display_path_color",
};

static short parse_user_color(SHELL_CONF *conf, FILE *fp, char key_buffer[50]);
static short parse_root_color(SHELL_CONF *conf, FILE *fp, char key_buffer[50]);
static short parse_display_path(SHELL_CONF *conf, FILE *fp, char key_buffer[50]);
static short parse_display_path_color(SHELL_CONF *conf, FILE *fp, char key_buffer[50]);
int shell_launch_pipes(char **args_1, char **args_2);

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
    size_t chris = sizeof(colorName) / 8;
    for (i = 0; i < chris; i++)
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

}

static short parse_display_path(SHELL_CONF *conf, FILE *fp, char key_buffer[50])
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

    if (strcmp(value_buffer, "true") == 0)
    {
        conf->show_path = 1;
        flag = 1;
    }
    else if (strcmp(value_buffer, "false") == 0)
    {
        conf->show_path = 0;
        flag = 1;
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
}
static short parse_display_path_color(SHELL_CONF *conf, FILE *fp, char key_buffer[50])
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
            conf->path_color = i;
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

}
int init_shell(SHELL_CONF **conf)
{
    short status = 0;

    *conf = (SHELL_CONF*)malloc(sizeof(SHELL_CONF));
    if (*conf == NULL)
        return -1;
    
    (*conf)->env = NULL;

    if (get_shell_owner(&((*conf)->env)) != 0)  // Know who launched the shell and his permissions.
        return -1;
    
    (*conf)->env->config_path = malloc(sizeof(char)*strlen(CONFIGFILE) + strlen((*conf)->env->home_dir_path) + 2);
    if ((*conf)->env->config_path == NULL)
        return -1;

    strcpy((*conf)->env->config_path, (*conf)->env->home_dir_path);
    strcat((*conf)->env->config_path, "/");
    strcat((*conf)->env->config_path, CONFIGFILE);
    
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
    int key_found = 0;
    int valid_key = 0;


    // Flags to control if we have already processed some parameters and avoid overwritting them.
    // We warn the user of redundancy in the config file.
    int flags[4] = {0, 0, 0, 0};


    fp = fopen(config->env->config_path, "r");
    if (fp == NULL)
    {
        config->root_color = 1;
        config->user_color = 2;
        config->show_path = 0;
        config->path_color = 4;

        fp = fopen(config->env->config_path, "wr");
        if (fp == NULL)
            return -2;
        else
        {
            fprintf(fp, "root_color=red\n");
            fprintf(fp, "user_color=green\n");
            fprintf(fp, "display_path=false\n");
            fprintf(fp, "display_path_color=blue\n");
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

        if (i != 0 && c == '\n' && config->warning_flag == 0)
        {
            key_buffer[i] = '\0';
            printf("%s", colorTypes[3]);
            printf("BSH WARNING : Unknown parameter \"%s\"\n", key_buffer);
            printf("%s", colorTypes[sizeof(colorTypes) / 8 - 1]);
        }
        else if (c == '=')
        {
            key_buffer[i] = '\0';

            /*
                * Check every possible parameters that are in the config file and process them 
                * We also check every flag to see the config doesn't contains multiple times the same parameter
                */
            
            for (i = 0, valid_key = 1; key_found != 1 && valid_key != 0 && i < (sizeof(possible_options)/8) ;i++)
            {
                if (strcmp(possible_options[i], key_buffer) == 0)
                {
                    key_found = 1;
                    if (flags[i] == 1 && config->warning_flag == 0)
                    {
                        printf("%s", colorTypes[3]);
                        printf("BSH WARNING : Redefinition of parameter \"%s\"\n", key_buffer);
                        printf("%s", colorTypes[sizeof(colorTypes) / 8 - 1]);
                        valid_key = 0;
                    }
                    else
                        valid_key = 1;
                }
            }
            if (key_found && valid_key)
            {
                if (strcmp(key_buffer, "user_color") == 0)
                {
                    if (parse_user_color(config, fp, key_buffer) == -1)
                        config->user_color = 2;

                    flags[1] = 1;

                }
                else if (strcmp(key_buffer, "root_color") == 0)
                {
                    if (parse_root_color(config, fp, key_buffer) == -1)
                        config->user_color = 1;
                
                    flags[0] = 1;
                }
                else if (strcmp(key_buffer, "display_path") == 0)
                {
                    if (parse_display_path(config, fp, key_buffer) == -1)
                        config->show_path = 0;

                    flags[2] = 1;
                }
                else if (strcmp(key_buffer, "display_path_color") == 0)
                {
                    if (parse_display_path_color(config, fp, key_buffer) == -1)
                        config->path_color = 4;
                }
                
            }
            else if (!key_found && config->warning_flag == 0)
            {
                printf("%s", colorTypes[3]);
                printf("BSH WARNING : Unknown parameter \"%s\"\n", key_buffer);
                printf("%s", colorTypes[sizeof(colorTypes) / 8 - 1]);
            }

            i = 0;
            key_found = 0;
            valid_key = 0;

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
    
    printf("%s", config->env->username);
    if (config->show_path == 1)
    {
        putchar(':');
        printf("%s", colorTypes[config->path_color]);
        printf("%s", config->env->curr_path);

    }
    printf("%s", colorTypes[sizeof(colorTypes) / 8 - 1]);   // Reset the color.
    printf("~$ ");
}


char *readCommandInput(void)
{
    int i, step, c;
    char *buffer;
    char *buffer_pt;

    buffer = NULL;
    i = 0;
    step = 2;

    buffer = (char*)malloc(sizeof(char)*SHELL_INPUT_BUFFER_SIZE);    // First alocate some memory by the ma buffer size
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
        if ((i + 1)% SHELL_INPUT_BUFFER_SIZE == 0)   // if buffer is full, we reallocate it by step*buffer_size.
        {
            buffer = (char*) realloc(buffer, sizeof(char*)*SHELL_INPUT_BUFFER_SIZE *step);
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

char **splitCommandInput(char *commandInput)
{
    int position, step;
    char **tokens = NULL;
    char *token = NULL;

    step = 2;
    position = 0;
    tokens = (char**) malloc(sizeof(char*)*SHELL_TOK_BUFFER_SIZE);
    
    if (tokens == NULL)
    {
        fprintf(stderr, "%s", colorTypes[1]);
        fprintf(stderr, "BSH : Memory Error !\n");
        fprintf(stderr, "%s", colorTypes[sizeof(colorTypes) / 8 -1]);
        return NULL;
    }

    token = strtok(commandInput, SHELL_TOK_DELIMITER);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if ((position+ 1) % SHELL_TOK_BUFFER_SIZE == 0)
        {
            tokens = realloc(tokens, sizeof(char*)*SHELL_TOK_BUFFER_SIZE *step);
            if (tokens == NULL)
            {
                fprintf(stderr, "%s", colorTypes[1]);
                fprintf(stderr, "BSH : Memory Error !\n");
                fprintf(stderr, "%s", colorTypes[sizeof(colorTypes) / 8 -1]);
                return NULL;
            }
        }
        token = strtok(NULL, SHELL_TOK_DELIMITER);
    }
    tokens[position] = NULL;
    return tokens;
}
int shell_launch_pipes(char **args_1, char **args_2)
{

    int pipefd[2]; 
    pid_t p1, p2;
  
    if (pipe(pipefd) < 0) {
        printf("\nPipe could not be initialized");
        exit(EXIT_FAILURE);
    }
    p1 = fork();
    if (p1 == 0) {
        // Child 1 executing..
        // It only needs to write at the write end
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
  
        if (execvp(args_1[0], args_1) < 0) {
            fprintf(stderr, "%s", colorTypes[1]);
            fprintf(stderr, "BSH : command not found !\n");
            fprintf(stderr, "%s", colorTypes[sizeof(colorTypes) / 8 -1]);
        }
        exit(EXIT_FAILURE);
    }   
  
    else if (p1 < 0) {
        fprintf(stderr, "%s", colorTypes[1]);
        fprintf(stderr, "BSH : Fork error !\n");
        fprintf(stderr, "%s", colorTypes[sizeof(colorTypes) / 8 -1]);
    }
    else {
        // Parent executing
        p2 = fork();
  
        if (p2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(args_2[0], args_2) < 0) {
                fprintf(stderr, "%s", colorTypes[1]);
                fprintf(stderr, "BSH : command not found !\n");
                fprintf(stderr, "%s", colorTypes[sizeof(colorTypes) / 8 -1]);
            }

            exit(EXIT_FAILURE);
        }
  
        // Child 2 executing..
        // It only needs to read at the read end
        if (p2 < 0) {
            fprintf(stderr, "%s", colorTypes[1]);
            fprintf(stderr, "BSH : Fork error !\n");
            fprintf(stderr, "%s", colorTypes[sizeof(colorTypes) / 8 -1]);
        }
        else {
            return 1;
        }
        return 1;
    }
    return 1;
}
int shell_launch(char **args)
{
    pid_t pid;
    int status;
    char **args_1, **args_2;
    int i, j, flag, second_count;

    flag = 1;
    i = 0;
    second_count = 0;

    while (args[i] != NULL && (flag = strcmp(args[i], "|")) != 0)
    {
        i++;
    }
    if (!flag)
    {
        args_1 = malloc(sizeof(char*)*i);
        j = 0;
        while (j < i)
        {
            args_1[j] = args[j];
            j++;
        }
        args_1[j] = NULL;
        i++;
        j = 0;

        while (args[i] != NULL)
        {
            i++;
            j++;
        }
        args_2 = malloc(sizeof(char*)*j);
        j++;

        while (j < i)
        {
            args_2[second_count] = args[j];
            second_count++;
            j++;
        }
        args_2[second_count] = NULL;
        shell_launch_pipes(args_1, args_2);
        return 1;
    }
    else
    {
        pid = fork();
        if (pid == 0)
        {
            if (execvp(args[0], args) == -1)
            {
                fprintf(stderr, "%s", colorTypes[1]);
                fprintf(stderr, "BSH : command not found !\n");
                fprintf(stderr, "%s", colorTypes[sizeof(colorTypes) / 8 -1]);
            }
            exit(EXIT_FAILURE);
        }
        else if (pid < 0)
        {
            fprintf(stderr, "%s", colorTypes[1]);
            fprintf(stderr, "BSH : Fork error !\n");
            fprintf(stderr, "%s", colorTypes[sizeof(colorTypes) / 8 -1]);
        }
        else
        {
            do {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
        return 1;
    }
    return 1;




}
void free_shell(SHELL_CONF *conf)
{
    assert(conf != NULL);
    free(conf->env->username);
    free(conf->env);
    free(conf);
}

int shell_execute(char **args, SHELL_CONF *config)
{
    if (strcmp(args[0], "cd") == 0)
    {
        return(cd_command(args, &(config->env)));
    }
    else if (strcmp(args[0], "exit") == 0)
    {
        return(exit_command(args));
    }
    
    return shell_launch(args);
}