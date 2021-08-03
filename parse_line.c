/*
 * parse_line.c
 *
 * breaks line entered by user into argument list
 * 
 * checks for '$$' variable placeholder
*/


#include "parse_line.h"
#include "constants.h"
#include "debug_printf.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

void parse_line(char* line, char** args)
{
    check_dolla_dolla(line);
    char *line_r;
    int i = 0;
    // break up the line along spaces
    char *token = strtok_r(line, " ", &line_r);
    while(token != NULL)
    {
        // populate the char** args array
        args[i] = calloc(strlen(token + 1), sizeof(char));
        strcpy(args[i], token);
        token = strtok_r(NULL, " ", &line_r);
        i++;
    } 
}


// Checks for variable place holder '$$'
// replaces with PID of smallsh
void check_dolla_dolla(char* string)
{
    if(string != NULL)
    {
        // allocate space for buffer
        char buf[MAX_INPUT_LENGTH];

        // Get smallsh pid and convert to string
        pid_t smallsh_pid = getpid();
        char pid_string[32];
        sprintf(pid_string, "%d", (int)smallsh_pid);
        DEBUG_PRINT("smallsh pid: %s\n", pid_string);

        for(int i = 0; i < (strlen(string) + 1); i++)
        {
            if(string[i] == '$')
            {
                if(string[i+1] == '$')
                {
                    // Zero out buffer for next iteration through loop
                    memset(buf, '\0', sizeof(buf));
                    // replace '$$' with pid of smallsh
                    // copy string contents into buffer up to the i'th location
                    strncpy(buf, string, i);
                    DEBUG_PRINT("Buf: %s\n", buf);
                    // tack on the smallsh pid to the buffer
                    strcat(buf, pid_string);
                    // tack on the rest of the string, without the "$$"
                    // the '2' is index the char* string past the "$$"
                    strcat(buf, string+i+2);
                    // copy contents of buff back into string
                    strcpy(string, buf);
                    DEBUG_PRINT("token string: %s\n", string);
                }
            }
        }
    }
}