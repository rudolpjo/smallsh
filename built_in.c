/*
 * build_in.c
*/


#include "debug_printf.h"
#include "built_in.h"
#include "process.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>


// checks the first argument for a 
// built in function
// executes if found with extra arguments if included 
int built_in_check(char** args)
{
    char* built_in_names[NUM_BUILT_IN];
    char buf[128];
    char path[128];
    int chdir_status = 0;
    char* home_dir = getenv("HOME");
    built_in_names[0] = "exit";
    built_in_names[1] = "cd";
    built_in_names[2] = "status";
    int func_flag = -1;
    for(int i = 0; i < NUM_BUILT_IN; i++)
    {
        if(strncmp(built_in_names[i], args[0], strlen(built_in_names[i])) == 0)
        {
            func_flag = i;
            break;
        }
    }
    switch (func_flag)
    {
    case -1: // function flag not set, built in function not passed
        return 0;
        break;
    case 0: // exit
        // TODO make function kill all other processes 
        // before killing top level smallsh
        printf("Exiting shell\n");
        exit(0);
        break;
    case 1: // cd
        // no additional agruments given
        // change to home directory
        if(args[1] == NULL)
        {   
            chdir_status = chdir(home_dir);
            DEBUG_PRINT("chdir_status for no args: %d\n", chdir_status);
            getcwd(buf, sizeof(buf));
            DEBUG_PRINT("%s\n", buf);
            fflush(NULL);
            memset(buf, 0, strlen(buf));
        }
        // argument is relative path
        // go to the path listed
        else if(args[1][0] != '/')
        {   
            DEBUG_PRINT("Relative path\n");
            getcwd(path, sizeof(path));
            strcat(path, "/");
            strcat(path, args[1]);
            chdir_status = chdir(path);
            getcwd(buf, sizeof(buf));
            DEBUG_PRINT("%s\n", buf);
            fflush(NULL);
            memset(buf, 0, strlen(buf));
        }
        // argument is absolute path
        // go to path starting in current directory
        else if(args[1][0] == '/')
        {
            chdir_status = chdir(args[1]);
            getcwd(buf, sizeof(buf));
            DEBUG_PRINT("%s\n", buf);
            fflush(NULL);
            memset(buf, 0, strlen(buf));       
        }
        if(chdir_status == -1)
        {
            printf("Error changing directory\n");
            fflush(NULL);
        }
        return 1;
        break;
    case 2: // status
        // prints  the exit status 
        // of the last foreground process ran by the shell
        printf("exit value %d\n", get_exit_status());
        return 1;
        break;
    default:
        return 0;
        break;
    }
    free(home_dir);
    return 0;
}
