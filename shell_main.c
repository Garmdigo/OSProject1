#include "shell_functions.h"

int main()
{
    // Initialize variables for traversing the background queue
    next = 0;
    imHigh = 0;

    //variable for calculating elapsed time using time.h
    struct timeval beginTime;

    //Start timing shell session
    gettimeofday(&beginTime, NULL);

    //initialize screen + token holder
    parseResult resultTokens;
    system("clear");

    while (1)
    {
        //retrieve initial tokens
        resultTokens = parse();
        //check if tokens contain env variables and resolve them
        parseEnv(resultTokens);
        //check if commands need to be expanded
        resultTokens.parseTokens[0] = prefixCommand(resultTokens.parseTokens[0]);
 
//	printTokens(resultTokens.parseTokens, resultTokens.tokenAmount);  
 
        //Builtins_________________________________________________________________________
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

        // CD
        int cd = strcmp(resultTokens.parseTokens[0], "cd");
        if (cd == 0)
        {
            //cdShell();
        }

        //(6)I/O Redirection_______________________________________________________________

	parseIO(resultTokens);	//check for <,> redirection



        /*
        
        resultTokens.parseTokens = The returned array with all the tokens
        
        resultTokens.tokenAmount = The amount of tokens in the array,
        so use this is as the limit when traversing through the array 
        
        Environment variable testing:
        printf("\n%s\n\n", environmentVariable(resultTokens.parseTokens[0]));

        */

        //(5) execution_____________________________________________________________________
        execute(resultTokens.parseTokens);

        //(7) Background Processing_________________________________________________________
        int ready = waitpid(background[0].pid, NULL, WNOHANG);
        /* TESTING
		char** sendMe = copyToks(resultTokens.parseTokens, resultTokens.tokenAmount - 1);
		int amount = resultTokens.tokenAmount - 1;
		send2back(sendMe, background, amount, imHigh, next);
		*/
    }

    return 0;
}
