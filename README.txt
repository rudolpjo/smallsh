
John Rudolph
rudolpjo@oregonstate.edu
November 3, 2020
CS 344 Operating Systems Fall 2020

*********************
smallsh
A small shell project
*********************

To build and run the project enter the following commands into the command line while in the project directory

make clean
make
./smallsh

If there is any compiler or linker script error, rerun 

make clean
make

You should now be in the shellsh, the prompt will be a <:>
Enter commands similar to other shells like Bash.

To exit the shell, enter

exit


To run the grading script, exit the shell and enter:

./p3testscript

smallsh will exit on completion of grading script

Note: For verbose execution of shell, remove the comment from
//#define DEBUG
in debug_printf.h and run make again

To all current students:
  Plagiarism of this will easily be detected.
