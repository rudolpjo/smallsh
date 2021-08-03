/*
 * process.h
 * 
 * signal handlers and helper functions 
*/


#include <sys/types.h>
#include <stdbool.h>

pid_t bg_proc_id[300];

void add_proc(pid_t id);
void sigtstp_handler(int status);
void sigchld_handler(int status);
void int_to_bytes(char* buffer, int num);
void store_exit_status(int status);
int get_exit_status();
bool get_bg_status();