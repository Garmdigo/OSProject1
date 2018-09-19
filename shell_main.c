#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "shell_functions.h"

int main()
{
    //variable for calculating elapsed time using time.h
    struct timeval beginTime;

    //Start timing shell session
    gettimeofday(&beginTime, NULL);

    //initialize screen + token holder
    parseResult resultTokens;
    system("clear");

    while (1)
    {
        resultTokens = parse();

        /*
        
        resultTokens.parseTokens = The returned array with all the tokens
        
        resultTokens.tokenAmount = The amount of tokens in the array,
        so use this is as the limit when traversing through the array 
        
        Environment variable testing:
        printf("\n%s\n\n", environmentVariable(resultTokens.parseTokens[0]));

        */

        //Builtins
        //exit
        int isExit = strcmp(resultTokens.parseTokens[0], "exit");
        if (isExit == 0)
        {
            exitShell(beginTime);
        }

        //echo
        int isEcho = strcmp(resultTokens.parseTokens[0], "echo");
        if (isEcho == 0)
        {
            echoShell(resultTokens.parseTokens, resultTokens.tokenAmount);
        }

	int cd = strcmp(resultTokens.parseTokens[0], "cd");
	if (cd == 0)
		printf("cd found.");
    }
    return 0;
}
