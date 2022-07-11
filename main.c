#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "shell.h"
#include "history.h"

void ignore_sigint(int sig_num);

void ignore_sigint(int sig_num)
{
    while (waitpid(-1, NULL, WNOHANG) > 0) {
	}
    printf("^C");
}
int main(int argc, char **argv, char **envp)
{
    SHELL_CONF *conf;

    int status;
    char *buffer;
    char **args;

    conf = NULL;
    status = 1;
    
    struct sigaction sa;
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = ignore_sigint;
    sa.sa_flags = 0;// not SA_RESTART!;


    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    if (init_shell(&conf, envp) != 0)
    {
        printf("%s", colorTypes[1]);
        printf("BSH ERROR : Memory error !\n");
        printf("%s", colorTypes[sizeof(colorTypes) / 8 -1]);
        exit(-1);
    }

    while(status)
    {

        show_prompt(conf);
        buffer = readCommandInput(conf);
        if (buffer != NULL){
            args = splitCommandInput(buffer);
            tcsetattr(STDIN_FILENO, TCSANOW, &conf->oldterm); // Restore before lauch
            status = shell_execute(args, conf);
        }
        if (status)
        {
            readShellConf(conf);
            tcsetattr(STDIN_FILENO, TCSANOW, &conf->newterm);
        }

    }

    save_local_hisory(conf->history);
    tcsetattr(STDIN_FILENO, TCSANOW, &conf->oldterm);

    free(buffer);
    free_shell(conf);
    
}