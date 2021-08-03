/*
 * sh.c
 * 
 * program loop where program is executed
*/


#include "sh.h"
#include "read_line.h"
#include "parse_line.h"
#include "constants.h"
#include "built_in.h"
#include "other_cmd.h"
#include "process.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>



void smallsh_exe()
{
    int status = 1;
    char *args[MAX_ARG_NUM];
    char line[MAX_INPUT_LENGTH];

    // set calling process to inform all 
    // child process upon termination of 
    // parent (calling) process, terminates
    // the child process
    // prctl(PR_SET_PDEATHSIG, SIGHUP);

    while(status != 0)
    {
        // zero out input buffers, assures the args array is NULL terminated
        memset(args, 0, sizeof(args));
        memset(line, 0, sizeof(line));

        // check for signals from child process to see if it has exited
        struct sigaction sa_sigchld;
        sa_sigchld.sa_handler = sigchld_handler;
        sa_sigchld.sa_flags = SA_RESTART;
        sigaction(SIGCHLD, &sa_sigchld, NULL);

        // check for SIGINT singal, ignore in this process
        struct sigaction sa_sigint;
        sa_sigint.sa_flags = 0;
        sa_sigint.sa_handler = SIG_IGN;
        sigaction(SIGINT, &sa_sigint, NULL);

        // check for SIGTSTP, pass signal to custom handler
        struct sigaction sa_sigtstp;
        sa_sigtstp.sa_flags = SA_RESTART;
        sa_sigtstp.sa_handler = sigtstp_handler;
        sigaction(SIGTSTP, &sa_sigtstp, NULL);

        fflush(NULL);
        // write prompt to console
        write(STDOUT_FILENO, ":", 1);
        // printf(":");
        fflush(NULL);
        // read in the line
        sh_read_line(line);
        // if the input is just 1 character long, 
        // only a newline is entered, and the program continues
        if(strlen(line) == 1)
        {
            continue;
        }
        // parse out the line to the args
        parse_line(line, args);
        // if the first character of the first arg
        // is '#' the line is a comment and ignored
        if(args[0][0] == '#')
        {
            continue;
        }
        // checks for and executes any built in commands
        // if it executes a built in, then the loop continues,
        // otherwise, the args are passed to call_others()
        if(built_in_check(args) == 1)
        {
            continue;
        }
        // function only called if command is not built in
        call_others(args);
    }
}
