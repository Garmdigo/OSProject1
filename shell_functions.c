#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <ctype.h>
#include "shell_functions.h"

//environment variable: Returns string with value of environemnt variable
//parameter string should be in the format: $VARIABLE (though lowercase is accepted)
char* environmentVariable(char* var)
{
    char capsVar[strlen(var)];
    //starts loop at [1] to disregard '$' changes var to uppercase
    for(int i = 1; i <= strlen(var); i++) {

        capsVar[i-1] = toupper(var[i]);
    }
    //testing value of capsvar: printf("\n%s\n", capsVar);
    return getenv(capsVar);
}

//built in functions
//exit
void exitShell(struct timeval beginTime)
{
    //needed variables
    struct timeval endTime;
    struct timeval startTime = beginTime;
    double elapsedTime;

    //get end time
    gettimeofday(&endTime, NULL);

    //get elapsed time
    elapsedTime = (endTime.tv_sec - startTime.tv_sec);      
    printf("\n%s\n%s%0.1f%s\n", "Exiting...", "Elapsed time: ", elapsedTime, "s");
    
    //exit
    exit (0);
}