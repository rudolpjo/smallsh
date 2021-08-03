/*
 * read_line.c
 * 
 * gets a line entered by user
 * from the command prompt
*/


#include "constants.h"
#include <stdio.h>
#include <string.h>

void sh_read_line(char* cmd_in)
{   
    char line[MAX_INPUT_LENGTH];
    fgets(line, MAX_INPUT_LENGTH, stdin);
    // trim new line character from input
    if(strlen(line) > 1)
    {
        memcpy(cmd_in, line, (strlen(line) - 1));
    }
    // don't trim input, only '\n' entered
    else
    {
        memcpy(cmd_in, line, (strlen(line)));
    }
}