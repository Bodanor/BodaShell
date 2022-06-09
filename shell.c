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
    "path_color",
    "display_hostname",
    "hostname_color"
};

static short parse_user_color(SHELL_CONF *conf, FILE *fp, char key_buffer[50]);
static short parse_root_color(SHELL_CONF *conf, FILE *fp, char key_buffer[50]);
static short parse_display_path(SHELL_CONF *conf, FILE *fp, char key_buffer[50]);
static short parse_path_color(SHELL_CONF *conf, FILE *fp, char key_buffer[50]);
static short parse_display_hostname(SHELL_CONF *conf, FILE *fp, char key_buffer[50]);
static short parse_hostname_color(SHELL_CONF *conf, FILE *fp, char key_buffer[50]);
static char **getArgs(char **args);
static void get_cur_pos(int *x, int *y);
static void remove_leftover_stdout(int nb_chars);

static void remove_leftover_stdout(int nb_chars)
{
    int x;

    for(x = nb_chars; x != 0; x--)
        printf("\b \b");
}
static void get_cur_pos(int *x, int *y)
{
    printf("\033[6n");  /* This escape sequence !writes! the current
                          coordinates to the terminal.
                          We then have to read it from there, see [4,5].
                          Needs <termios.h>,<unistd.h> and some others */
   scanf("\033[%d;%dR", y, x);

}
static int runSpecialKey(char key, SHELL_CONF *conf, SHELL_HISTORY *history);

static int runSpecialKey(char key, SHELL_CONF *conf, SHELL_HISTORY *history)
{
    if (key == '\033')
    {
        getchar();
        switch (getchar())
        {
        case 'A':
            browse_history_up(history);
            return 1;
            break;
        case 'B':
            browse_history_down(history);
            return 1;
            break;
        case 'C':
            printf("RIGHT");
            break;
        case 'D':
            printf("LEFT");
            break;
        }
    }

    return 0;

}
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
            printf("BSH WARNING : Unknown value \"%s\" with parameter \"%s\" ! Defaulting to blue\n", value_buffer, key_buffer);
            printf("%s", colorTypes[sizeof(colorTypes) / 8 - 1]);
        }
        return -1;
    }
    return 0;
}
static short parse_display_hostname(SHELL_CONF *conf, FILE *fp, char key_buffer[50])
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
        conf->show_hostname = 1;
        flag = 1;
    }
    else if (strcmp(value_buffer, "false") == 0)
    {
        conf->show_hostname = 0;
        flag = 1;
    }

    if (flag == 0)  // If no value has been found then we warn the user that default settings are applied.
    {
        if (conf->warning_flag == 0)
        {
            printf("%s", colorTypes[3]);
            printf("BSH WARNING : Unknown value \"%s\" with parameter \"%s\" ! Defaulting to green\n", value_buffer, key_buffer);
            printf("%s", colorTypes[sizeof(colorTypes) / 8 - 1]);
        }
        return -1;
    }
    return 0;

}
static short parse_hostname_color(SHELL_CONF *conf, FILE *fp, char key_buffer[50])
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
            conf->hostname_color = i;
            flag = 1;
        }
    }

    if (flag == 0)  // If no value has been found then we warn the user that default settings are applied.
    {
        if (conf->warning_flag == 0)
        {
            printf("%s", colorTypes[3]);
            printf("BSH WARNING : Unknown value \"%s\" with parameter \"%s\" ! Defaulting to green\n", value_buffer, key_buffer);
            printf("%s", colorTypes[sizeof(colorTypes) / 8 - 1]);
        }
        return -1;
    }
    return 0;


}
static short parse_path_color(SHELL_CONF *conf, FILE *fp, char key_buffer[50])
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
int init_shell(SHELL_CONF **conf, char **envp)
{
    short status = 0;

    *conf = (SHELL_CONF*)malloc(sizeof(SHELL_CONF));
    if (*conf == NULL)
        return -1;
    
    (*conf)->env = NULL;

    tcgetattr(STDIN_FILENO, &(*conf)->oldterm);
    (*conf)->newterm = (*conf)->oldterm;
    (*conf)->newterm.c_lflag &= ~( ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &(*conf)->newterm);

    if (get_shell_owner(&((*conf)->env)) != 0)  // Know who launched the shell and his permissions.
        return -1;
    
    (*conf)->env->config_path = malloc(sizeof(char)*strlen(CONFIGFILE) + strlen((*conf)->env->home_dir_path) + 2);
    if ((*conf)->env->config_path == NULL)
        return -1;

    strcpy((*conf)->env->config_path, (*conf)->env->home_dir_path);
    strcat((*conf)->env->config_path, "/");
    strcat((*conf)->env->config_path, CONFIGFILE);
    
    (*conf)->env->envp = (char **)malloc(sizeof(char*));
    if ((*conf)->env->envp == NULL)
        return -1;
    
    (*conf)->env->envp = envp;
    (*conf)->warning_flag = 0;  // Set the warning flag to zero to allow at least one time to warn user for config errors.
    // INIT HISTORY
    // Vérifier output de init_history
    
    if (init_history(&(*conf)->history, (*conf)->env->home_dir_path) == -1)
    {
        printf("%s", colorTypes[2]);
        printf("BSH ERROR : HISTORY MEMORY ERROR\n");
        printf("%s", colorTypes[sizeof(colorTypes) / 8 - 1]);
    }

    load_history((*conf)->history);
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
    int flags[6] = {0, 0, 0, 0, 0, 0};


    fp = fopen(config->env->config_path, "r");
    if (fp == NULL)
    {
        config->root_color = 1;
        config->user_color = 2;
        config->show_path = 0;
        config->path_color = 4;
        config->show_hostname = 0;
        config->hostname_color = 2;

        fp = fopen(config->env->config_path, "wr");
        if (fp == NULL)
            return -2;
        else
        {
            fprintf(fp, "root_color=red\n");
            fprintf(fp, "user_color=green\n");
            fprintf(fp, "display_path=false\n");
            fprintf(fp, "path_color=blue\n");
            fprintf(fp, "display_hostname=false\n");
            fprintf(fp, "hostname_color=green\n");
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
                else if (strcmp(key_buffer, "path_color") == 0)
                {
                    if (parse_path_color(config, fp, key_buffer) == -1)
                        config->path_color = 4;
                }
                else if (strcmp(key_buffer, "display_hostname") == 0)
                {
                    if(parse_display_hostname(config, fp, key_buffer) == -1)
                        config->show_hostname = 0;
                }
                else if (strcmp(key_buffer, "hostname_color") == 0)
                {
                    if (parse_hostname_color(config, fp, key_buffer) == -1)
                        config->hostname_color = 2;
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
    if (config->env->hostname != NULL && config->show_hostname == 1)
    {
        putchar('@');
        printf("%s", colorTypes[config->hostname_color]);
        printf("%s", config->env->hostname);

    }
    if (config->env->curr_path != NULL && config->show_path == 1)
    {
        putchar(':');
        printf("%s", colorTypes[config->path_color]);
        printf("%s", config->env->curr_path);

    }
    printf("%s", colorTypes[sizeof(colorTypes) / 8 - 1]);   // Reset the color.
    printf("~$ ");
}


char *readCommandInput(SHELL_CONF *conf)
{
    int c, arrow_flag;
    size_t line_size;
    char *new = NULL;

    
    line_size = 0;
    arrow_flag = 0;
    get_cur_pos(&conf->coordinates.begin_line_x, &conf->coordinates.begin_line_y);    // Get beginning of line
    conf->coordinates.curr_x = conf->coordinates.begin_line_x;
    conf->coordinates.curr_y = conf->coordinates.begin_line_y;
    
    conf->history->history_commands[conf->history->current_index] = (char*)malloc(sizeof(char)*SHELL_INPUT_BUFFER_SIZE);
    
    if (conf->history->history_commands[conf->history->current_index] == NULL)
        //ERROR HANDLING
        ;
    *conf->history->history_commands[conf->history->current_index] = '\0';

    while ((c = getchar()) != '\n')
    {
        arrow_flag = runSpecialKey(c, conf, conf->history);
        if (arrow_flag)     // If arrow keys are pressed, we handle them
        {   
            remove_leftover_stdout(conf->coordinates.curr_x- conf->coordinates.begin_line_x); // Remove leftover input from previous arrows
            printf("%s", conf->history->history_commands[conf->history->current_index]);
            get_cur_pos(&conf->coordinates.curr_x, &conf->coordinates.curr_y); // Update after printing 
        }
        else
        {
            tcsetattr(STDIN_FILENO, TCSANOW, &conf->oldterm);
            
            line_size = strlen(conf->history->history_commands[conf->history->current_index]) + 1;
            if ((line_size + 1) % SHELL_INPUT_BUFFER_SIZE == 0)
            {
                new = realloc(conf->history->history_commands[conf->history->current_index], sizeof(char)*line_size);
                conf->history->history_commands[conf->history->current_index] = new;
            }

            arrow_flag = 0;
            conf->history->history_commands[conf->history->current_index][line_size - 1] = c;
            conf->history->history_commands[conf->history->current_index][line_size++] = '\0';
            putchar(c);
        } 
        tcsetattr(STDIN_FILENO, TCSANOW, &conf->newterm);
    }

    putchar('\n');

    tcsetattr(STDIN_FILENO, TCSANOW, &conf->oldterm);
    if (*conf->history->history_commands[conf->history->current_index] != 0)
        return conf->history->history_commands[conf->history->current_index];
    else
        return NULL;

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

static char **getArgs(char **args)
{
    char **args_tmp = NULL;
    int i;
    static char **curr_pos_each_call = NULL;

    if (args != NULL)
    {
        if (curr_pos_each_call == NULL)
            curr_pos_each_call = args;

        for(i = 0; curr_pos_each_call[i] != NULL && strcmp(curr_pos_each_call[i], "|") != 0; i++)
        {
            args_tmp = (char**)realloc(args_tmp, sizeof(char*)*(i + 2));
            args_tmp[i] = curr_pos_each_call[i];
        }
        
        if (i != 0)
        {
            if (curr_pos_each_call[i] != NULL && strcmp(curr_pos_each_call[i], "|") == 0)
                curr_pos_each_call += (i + 1);
            else
                curr_pos_each_call += i;
            args_tmp[i] = NULL;
        }
        

    }
    else
    {
        curr_pos_each_call = NULL;
    }
    return args_tmp;

}

int shell_launch(char **args)
{
    int   p[2];
    pid_t pid;
    int   fd_in = 0;
    char **tmp = NULL;
    int n = 0;
    int i = 1;
    int j = 0;
    
    for (n = 0; args[n] != NULL ; n++)
    {
        if (strcmp(args[n], "|") == 0)
            i++;

    }
    
    while (j < i)
    {
        tmp = getArgs(args);

        if (pipe(p) < 0)
        {
            fprintf(stderr, "%s", colorTypes[1]);
            fprintf(stderr, "BSH : Pipe Error !\n");
            fprintf(stderr, "%s", colorTypes[sizeof(colorTypes) / 8 -1]);
        }
        if ((pid = fork()) == -1)
        {
            fprintf(stderr, "%s", colorTypes[1]);
            fprintf(stderr, "BSH : Fork Error !\n");
            fprintf(stderr, "%s", colorTypes[sizeof(colorTypes) / 8 -1]);
        }
        else if (pid == 0)
        {
            dup2(fd_in, 0);         // fd_in result before pipe
            //close(fd_in);           // STDIN = fd_in
            if (j + 1!= i)
            {
                dup2(p[1], 1);      // If we are last, thus last command atfer pipe --> STDOUT = pipe_write
            }
            close(p[1]);            // Closing pipe_write
            close(p[0]);            // Closing pipe_read

            if(execvp(tmp[0], tmp) < 0)
            {
                fprintf(stderr, "%s", colorTypes[1]);
                fprintf(stderr, "BSH : %s : command not found !\n", tmp[0]);
                fprintf(stderr, "%s", colorTypes[sizeof(colorTypes) / 8 -1]);
            }
            exit(EXIT_FAILURE);
        }
        else
        {
            if (j + 1 == i)
            {     
                close(p[0]);
                close(p[1]);
            }
            else
                close(p[1]);
                
            wait(NULL);

            fd_in = p[0]; // Save the input for the next command
            j++;
        }
    }

    getArgs(NULL);
    free(args);
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
    else if (strcmp(args[0], "show-env") == 0)
    {
        return(show_env_command(args, config->env));
    }
    
    return shell_launch(args);
}