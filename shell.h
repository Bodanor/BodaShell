#ifndef __SHELL_H__
#define __SHELL_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "env.h"
#include "commands.h"

#define CONFIGFILE ".bodashell.conf"
#define SHELL_INPUT_BUFFER_SIZE 256
#define SHELL_TOK_BUFFER_SIZE 256
#define SHELL_TOK_DELIMITER " \t\r\n\a"

static const char *colorTypes[] = {
    "\033[1;30m",   //Black
    "\033[1;31m",   //Red
    "\033[1;32m",   //Green
    "\033[1;33m",   //Yellow
    "\033[1;34m",   //Blue
    "\033[1;35m",   //Purple
    "\033[1;36m",   //Cyan
    "\033[1;37m",   //White
    "\033[0m",      //RESET

};


typedef struct SHELL_CONF_t
{
    short root_color;
    short user_color;
    short path_color;
    short show_path;
    short warning_flag;
    ENV_t *env;


}SHELL_CONF;

/**
 * @brief Initialize the shell with some parameters if defined.
 * 
 * @param conf Double pointer Shell conf structure that contains the parameters
 * @post: *conf pointing so valid memory containing shell parameters fot it to work properly
 * @return:
 *          0 : If successfull
 *          -1 : If memory error
 */
int init_shell(SHELL_CONF **conf);

/**
 * @brief Read the config file if does exist.
 * 
 * @param config Shell conf structure that contains the parameters
 * @pre: @param config Pointing to a valid memory location
 * @post: valid config parameter if found.
 * @return:
 *        0: If successfull
 *       -1: If error with the config file.
 */
short readShellConf(SHELL_CONF *config);

/**
 * @brief show prompt with user attributes to the screen that invites him to enter some commands
 * 
 * @param config Shell conf structure that contains the parameters
 * @post: @param config Pointing to a valid memory location
 * @pre: Print prompt to screen with proper attribute for the user
 * @return nothing
 */
void show_prompt(SHELL_CONF *config);

/**
 * @brief Get the user input command 
 * 
 * @return:
 *         NULL if unput is empty or a memory error occured
 *         char * to the user's command.
 */
char *readCommandInput(void);

/**
 * @brief Free all the allocated memory to avoid memory leak
 * 
 * @param conf Shell conf structure that contains the parameters
 * @pre @param conf Pointing to a valid memory location
 * @post free all allocated memory
 */
void free_shell(SHELL_CONF *conf);

char **splitCommandInput(char *commandInput);
int shell_launch(char **args);
int shell_execute(char **args, SHELL_CONF *config);
int check_for_piping(char **args);
#endif