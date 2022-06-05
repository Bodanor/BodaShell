#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "shell.h"

void ignore_sigint(int sig_num);

void ignore_sigint(int sig_num)
{
    while (waitpid(-1, NULL, WNOHANG) > 0) {
	}
	printf("\n");;
}
int main(int argc, char **argv)
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

    if (init_shell(&conf) != 0)
    {
        printf("%s", colorTypes[1]);
        printf("BSH ERROR : Memory error !\n");
        printf("%s", colorTypes[sizeof(colorTypes) / 8 -1]);
        exit(-1);
    }

    while(status)
    {

        show_prompt(conf);
        buffer = readCommandInput();
        if (buffer != NULL){
            args = splitCommandInput(buffer);
            status = shell_execute(args, conf);
        }
        readShellConf(conf);
    }
    free(buffer);
    free_shell(conf);
    
    

}