#include "shell_functions.h"

int main()
{
    // Initialize variables for traversing the background queue
    next = 0;
    imHigh = 1;
	capacity = 20;

	queue = (Process*)malloc(capacity * sizeof(Process));
	
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

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	parseIO(resultTokens);	//check for <,> redirection
	parsePipe(resultTokens); 	//check for pipes
=======
=======
>>>>>>> 381ea52cfed847a2c4cdacd097fb1f22ae3e25d1
=======
>>>>>>> 381ea52cfed847a2c4cdacd097fb1f22ae3e25d1
		parseIO(resultTokens);	//check for <,> redirection

>>>>>>> 381ea52cfed847a2c4cdacd097fb1f22ae3e25d1
=======
		parseIO(resultTokens);	//check for <,> redirection

<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> 381ea52cfed847a2c4cdacd097fb1f22ae3e25d1

=======
>>>>>>> 381ea52cfed847a2c4cdacd097fb1f22ae3e25d1
=======
>>>>>>> 381ea52cfed847a2c4cdacd097fb1f22ae3e25d1
        //(5) execution_____________________________________________________________________
        
		//if(resultTokens.tokenAmount == 1)
			//execute(resultTokens.parseTokens);

        //(7) Background Processing_________________________________________________________
        
		// If isBackground() == true
		char** sendMe = copyToks(resultTokens.parseTokens, resultTokens.tokenAmount - 1);
		int amount = resultTokens.tokenAmount - 1;
		send2back(sendMe, queue, amount, imHigh, next);
		next++;
		imHigh++;
		
		// Check to see if the next background process in the queue has terminated.
		if(next != 0)
		{
			int ready = waitpid(queue[0].pid, NULL, WNOHANG);
			if(ready == queue[0].pid)
			{
				// Background process finished

				finish(queue, next);
				next--;
				
				if(next == 0)
				{
					// Reset imHigh when the queue becomes empty.
					imHigh = 1;
				}	
			}
		}
    }
	
	free(queue);
    return 0;
}
