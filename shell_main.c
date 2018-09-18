#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "shell_functions.h"

int main() {
    //variable for calculating elapsed time using time.h
    struct timeval beginTime;
    
    //Start timing shell session
    gettimeofday(&beginTime, NULL);

    //initialize screen + token holder
    parseResult resultTokens;
    system("clear");
    
    while (1) {
        resultTokens = parse();
        
        /*
        
        resultTokens.parseTokens = The returned array with all the tokens
        
        resultTokens.tokenAmount = The amount of tokens in the array,
        so use this is as the limit when traversing through the array 
        
        */

        //Builtins
        //exit
        int isExit = strcmp(resultTokens.parseTokens[0], "exit");
        if (isExit == 0)
        {
            exitShell(beginTime);
        }
        
        /* Environment variable testing
        
        printf("\n%s\n\n", environmentVariable(resultTokens.parseTokens[0]));

        */
    }
    return 0;
}
