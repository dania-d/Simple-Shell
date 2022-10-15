/**************************************************************
* Class:  CSC-415-03 Fall 2022
* Name: Dania Dababo
* Student ID: 922001571
* GitHub ID: dania-d
* Project: Assignment 3 – Simple Shell
*
* File: Dababo_Dania_HW3_main.c
*
* Description: Implement a simple shell that runs on top of the
* regular command-line interpreter for linux
*
**************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 123 // no more than 123 bytes to be fetched
#define MAX_ARGS 6  // must handle at least 4 args, filename, NUll 
char strIn[MAX_INPUT];  // user input
char *args[MAX_ARGS]; // split user input
//pid_t pid;

int main (int argc, char *argv[]) {
// loop until we get valid input?
while(1) {
    if(argc > 1) {   // specified prompt
        printf("%s ", argv[1]);
    } else {
        printf("\n>");    // default prompt
    }
    //get user input, up to 123 bytes
    fgets(strIn, MAX_INPUT, stdin); 
    // remove \n at end of strInfer in order to pass args[0] to execvp
    strIn[strcspn(strIn, "\n")] = '\0';

    if (strcmp(strIn, "exit") == 0) {  // return to reg shell on 'exit'
        printf("Goodbye!\n");
        exit(0);
    }
    if (feof(stdin)) {  // EOF, exit shell
        exit(0);
    }
    int userInputCount = 0; // keep track of index in args[]
    char *token = strtok(strIn, " "); // parse user input into substrings
    while (token != NULL) { // store all non-NULL tokens in array
        args[userInputCount] = token; // starts at index 0
        userInputCount++;   // increment so next token can be at next index
        token = strtok(NULL, " ");  // get the next token
    }
    //add null to first empty index so we can count number of args
    args[userInputCount] = NULL;
    if(userInputCount == 0){    // if count is 0, user entered empty line
        printf("Please enter valid command\n");
    }

    pid_t pid;
    pid = fork(); // create copy of calling process
    int status;

    if (pid == 0) { // we are in the child proces
        int execRtn = execvp(args[0], args); // overwrite predecessor process
        if  (execRtn == -1) { // execvp unsuccessful if -1 returned
            printf("execvp failed\n");
            exit(1);
        }
    } else if (pid < 0) {   // fork unsuccessful if neg. value returned
        printf("Fork Failed");
    } else {    // fork returned pos. value; returned to parent
        wait(&status);    // parent must wait for child
        printf("Child %d, exited with %d\n", pid, WEXITSTATUS(status));
    }

} // end while loop
return 0;
}
