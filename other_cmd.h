/*
 * other_cmd.h
*/


#include "constants.h"

#include <stdbool.h>


typedef struct cmd_args
{
    char *argv[MAX_ARG_NUM];
    char input_file[MAX_INPUT_LENGTH];
    char output_file[MAX_INPUT_LENGTH];
    bool background;
} cmd_args;

int call_others(char** args);
void init_cmd_args(cmd_args* cmds, char** args);
void free_cmd_args(cmd_args* cmds);