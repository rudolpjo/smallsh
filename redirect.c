/*
 * redirect.c
 * 
 * handles the checking for IO redirect
 * dictated by the user input
*/


#include "other_cmd.h"
#include "constants.h"
#include "debug_printf.h"

#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BG_REDIRECT "/dev/null"


void redirect(cmd_args* cmds)
{
    // If there is a file to redirect to
    // try the redirect
    // set permission to rw for all 
    if(strlen(cmds->input_file) > 0)
    {
        // set input redirect to cmds->input file
        int in_fd = open(cmds->input_file, O_RDONLY, 0666);
        if(in_fd < 0)
        {
            printf("cannot open %s for input\n", cmds->input_file);
            fflush(NULL);
            exit(EXIT_FAILURE); // set status to Fail, exit child process
        }
        else
        {
            DEBUG_PRINT("Redirecting file input\n");
            dup2(in_fd, STDIN_FILENO);
        }
        close(in_fd);
    }
    // if a redirect file has not been allocated
    // and the process if for the background, 
    // redirect to /dev/null
    // set permission to rw for all 
    else if ((strlen(cmds->input_file) <= 0) && (cmds->background == true))
    {
        int in_fd = open(BG_REDIRECT, O_RDONLY);
        if(in_fd < 0)
        {
            perror("Error opening input redirect file\n");
            fflush(NULL);
            exit(EXIT_FAILURE); // set status to Fail, exit child process
        }
        else
        {
            DEBUG_PRINT("Redirecting file input to /dev/null\n");
            dup2(in_fd, STDIN_FILENO);  // redirect input file
        }
        close(in_fd);
    }
    // If there is a file to redirect to
    // try the redirect
    if(strlen(cmds->output_file) > 0)
    {
        // set output redirect to cmds->output file
        // file is open write only, truncated if exists, 
        // created if it does not exist
        // set permission to rw for all 
        printf("output file: %s\n", cmds->output_file);
        int out_fd = open(cmds->output_file, O_WRONLY | O_TRUNC | O_CREAT, 0666);
        if(out_fd < 0)
        {
            perror("Error opening output redirect file\n");
            fflush(NULL);
            exit(EXIT_FAILURE); // set status to Fail, exit child process 
        }
        else
        {
            DEBUG_PRINT("Redirecting file output\n");
            int fd_status = dup2(out_fd, STDOUT_FILENO);
            if(fd_status < 0)
            {
                perror("Error dup2 redirect input\n");
            }
        }
        close(out_fd);
    }
    // if a redirect file has not been allocated
    // and the process if for the background, 
    // redirect to /dev/null
    else if((strlen(cmds->output_file) <= 0) && (cmds->background == true))
    {
        // process is background and redirect not set 
        // so  set output redirect to /dev/null 
        int out_fd = open(BG_REDIRECT, O_WRONLY);
        if(out_fd < 0)
        {
            printf("cannot open %s for output\n", cmds->output_file); 
            fflush(NULL);
            exit(EXIT_FAILURE); // set status to Fail, exit child process
        }
        else
        {
            DEBUG_PRINT("Redirecting file output\n");
            int fd_status = dup2(out_fd, STDOUT_FILENO);
            if(fd_status < 0)
            {
                perror("Error dup2 redirect output\n");
            }
        }
        close(out_fd);
    }
}
