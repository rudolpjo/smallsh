/*
 * debug_printf.h
 * 
 * Debug helper, allows for turning off
 * all DEBUG_PRINT() statements
*/

#ifndef SRC_DEBUG_PRINTF_H_ 
#define SRC_DEBUG_PRINTF_H_ 
#include <stdio.h> 

// #define DEBUG
#ifdef DEBUG
#define DEBUG_PRINT(...)  fprintf( stderr, __VA_ARGS__ ); 
#else
#define DEBUG_PRINT(...) 
#endif

#endif /* SRC_DEBUG_PRINTF_H_ */

