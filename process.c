/*
 * process.c
 * 
 * signal handlers and helper functions 
*/


#include "process.h"

#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

// global flags 
// treated like private class data
// only accessed through get/set functions or locally
int proc_index = 0;
int exit_status = 0;
bool bg_status = true;


// adds a pid_t to the array
void add_proc(pid_t id)
{
    if(proc_index > 299)
    {
        printf("Too many processes!\n");
    }
    bg_proc_id[proc_index] = id;
    proc_index++;
}


// handler for SIGTSTP
// toggles the bg_status flag,
// writes appropriate message
void sigtstp_handler(int status)
{
    bg_status = !bg_status;
    if(bg_status == true)
    {
        write(STDOUT_FILENO, "Exiting foreground-only mode\n", 29);
    }
    else
    {
        write(STDOUT_FILENO, "Entering foreground-only mode (& is now ignored)\n", 49);
    }
    fflush(NULL);
}

// handler for SIGCHLD signal
void sigchld_handler(int status)
{
    pid_t pid;
    while((pid = waitpid(-1, &status, WNOHANG))>0)
    {
        for(int i = 0; i < 300; i++)
        {
            if(pid == bg_proc_id[i])
            {
                // case where process exits normally
                // get and write exit status
                if(WIFEXITED(status))
                {
                    int exit_status = WEXITSTATUS(status);
                    char stat_buf[19];
                    char pid_buffer[19];
                    memset(stat_buf, '\0', 19);
                    memset(pid_buffer, '\0', 19);
                    int_to_bytes(stat_buf, exit_status);
                    int_to_bytes(pid_buffer, (int)pid);
                    write(STDOUT_FILENO, "background pid ", 15);
                    write(STDOUT_FILENO, &pid_buffer, 19);
                    write(STDOUT_FILENO, " is done: exit value ", 21);
                    if(exit_status == 0)
                    {
                        write(STDOUT_FILENO, &stat_buf, 2);
                    }
                    else
                    {
                        write(STDOUT_FILENO, &stat_buf, 19);
                    }
                }
                // case where process exits with a signal interrupt
                // get and write signal value
                else if(WIFSIGNALED(status))
                {
                    int sig_num = WTERMSIG(status);
                    char pid_buffer[19];
                    char sig_buf[19];
                    memset(pid_buffer, '\0', 19);
                    memset(sig_buf, '\0', 19);
                    int_to_bytes(pid_buffer, (int)pid);
                    int_to_bytes(sig_buf, (int)sig_num); 
                    write(STDOUT_FILENO, "background pid ", 15);
                    write(STDOUT_FILENO, &pid_buffer, 19);
                    write(STDOUT_FILENO, " is done: terminated by signal ", 31);
                    write(STDOUT_FILENO, &sig_buf, 19);
                }
                fflush(NULL);
                return;
            }
        }
    }
}

// cat /proc/sys/kernel/pid_max give max PID of 327680
// length of buffer set to accomodate largest PID
// Function performs conversion from digits to ascii character
// places each ascii character in byte array for write()
void int_to_bytes(char* buffer, int num)
{
    if(num == 0)
    {
        buffer[0] = 48;
        buffer[1] = 10; // newline to end of buffer
        for(int i = 2; i<18; i++)
        {
            buffer[i] = 48;   
        }
    }
    else
    {
        int i = 18;
        // if the number is <32, it is a signal, not a PID
        // signal is the last print, so give it a '\n'
        if(num < 32)
        {
            buffer[18] = 10;   // new line to end of buffer
            i = 17;
        }
        while(num != 0 || i < 0)
        {
            buffer[i] = ((num % 10) + 48) & 0xFF;
            num = num/10;
            i--;
        }
    }
}

void store_exit_status(int status)
{
    exit_status = status;
}

int get_exit_status()
{
    return exit_status;
}

bool get_bg_status()
{
    return bg_status;
}