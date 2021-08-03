/* 
 * other_cmd.c
 * 
 * process commands that are not built in
*/


#include "debug_printf.h"
#include "other_cmd.h"
#include "constants.h"
#include "process.h"
#include "redirect.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/prctl.h>


//
// Processes arguments that are not comments or build in functions
//
int call_others(char** args)
{
    // creats a cmd_args struct and initial it 
    // by placing args into cmd_args struct 
    cmd_args* cmds = calloc(1, sizeof(cmd_args));
    init_cmd_args(cmds, args);

    // create child process that will 
    // executes the given command
    pid_t pid = fork();
    // fork fail
    if(pid < 0)
    {
        perror("Fork failed\n");
        return EXIT_FAIL;
    }
    // child fork
    else if(pid == 0)
    {
        // Set child process to receive SIGHUP
        // if calling process terminates 
        prctl(PR_SET_PDEATHSIG, SIGHUP);
       
        // check for input/output redirection
        redirect(cmds);

        // if the process is running in the foreground,
        // set SIGINT to its default settings
        if(cmds->background == false)
        {
            struct sigaction fg_sa_sigint;
            fg_sa_sigint.sa_handler = SIG_DFL;
            sigfillset(&fg_sa_sigint.sa_mask);
            fg_sa_sigint.sa_flags = SA_RESTART;
            sigaction(SIGINT, &fg_sa_sigint, NULL);
        }
        // Run the new program in the child fork
        if(execvp(cmds->argv[0], cmds->argv) < 0)
        {
            // if execvp fails, the commands is not in the PATH
            printf("%s: no such file or directory\n", cmds->argv[0]);
            fflush(NULL);
            exit(EXIT_FAILURE); // set status to Fail, exit child process
        } 
    }
    // parent fork, the above execvp, or exiting on failure
    // keeps the child process from executing this code
    //
    // check for background task bool
    if(cmds->background == false)
    {
        // since cmds->background is not set,
        //wait for child process
        int status;
        waitpid(pid, &status, 0);
        // if child process exits normally,
        // record its status in pid_t arrray
        if(WIFEXITED(status) == true)
        {
            int child_exit_status = WEXITSTATUS(status);
            store_exit_status(child_exit_status);
            if (child_exit_status != 0)
            {
                DEBUG_PRINT("Error: child exit status: %d\n", child_exit_status);
            }
            else
            {
                DEBUG_PRINT("Success: child exit status: %d\n", child_exit_status);
            }
        }
        // if child process exits because of a signal,
        // print out the message and signal number
        else if(WIFSIGNALED(status) == true)
        {
            int signal = WTERMSIG(status);
            printf("terminated by signal %d\n", signal);
        }
    }
    // the child process is in the background,
    // proceed without waiting
    else
    {
        printf("background PID is %d\n", pid);
        fflush(NULL);
        // add pid to list of pid's 
        add_proc(pid);
        DEBUG_PRINT("Not waiting for child process\n");
    }

    free_cmd_args(cmds);
    // return from function back to sh loop
    return EXIT_SUCCESS;
}


// 
// Initialized the cmd_args struct
//
void init_cmd_args(cmd_args* cmds, char** args)
{
    memset(cmds->output_file, '\0', sizeof(cmds->output_file));            
    memset(cmds->input_file, '\0', sizeof(cmds->input_file));
    // set background to be false by default
    cmds->background = false;
    int i = 0;
    // loop through the args
    while(args[i] != NULL)
    {
        DEBUG_PRINT("Arg %i: %s\n", i, args[i]);
        // populate the output redirect field with
        // the next argument
        if(strcmp(args[i], ">") == 0)
        {
            strcpy(cmds->output_file, args[i+1]);
        }
        // populate thtet input redirect field 
        // with the next argument
        else if(strcmp(args[i], "<") == 0)
        {
            strcpy(cmds->input_file, args[i+1]);
        }
        // background flag (&) found, 
        // check if ^Z (SIGTSTP) has changed the 
        // foreground/background status
        else if(strcmp(args[i], "&") == 0)
        {
            if(get_bg_status() == true)  // returns true if ^Z has not been asserted
            {
                DEBUG_PRINT("Background process set\n");
                cmds->background = true;
            }
        }
        // place the commands, not the flags, into the cmds->argv array
        else
        {
            cmds->argv[i] = calloc(1, strlen(args[i]));
            strcpy(cmds->argv[i], args[i]);
        }
        i++;
    } // end while loop 
}


void free_cmd_args(cmd_args* cmds)
{
    memset(cmds->output_file, '\0', sizeof(cmds->output_file));            
    memset(cmds->input_file, '\0', sizeof(cmds->input_file));
    int i = 0;
    while(cmds->argv[i] != NULL)
    {
        free(cmds->argv[i]);
        i++;
    }
    free(cmds);
}
