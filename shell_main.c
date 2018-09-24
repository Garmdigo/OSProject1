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
        resultTokens.parseTokens[0] = resolvePath(resultTokens.parseTokens[0]);
 
	//printTokens(resultTokens.parseTokens,resultTokens.tokenAmount);

		isAmp = 0;
		isDire = 0;
		isPipe = 0;
		isEcho = 0;
		isExit = 0;
		isCd = 0;
		isIo = 0;
		
		// Count the special characters.
		for(int i = 0; i < resultTokens.tokenAmount; i++)
		{
			if(strcmp(resultTokens.parseTokens[i], "&") == 0)
				isAmp++;
			else if(strcmp(resultTokens.parseTokens[i], "<") == 0 || strcmp(resultTokens.parseTokens[i], ">") == 0)
				isDire++;
			else if(strcmp(resultTokens.parseTokens[i], "|") == 0)
				isPipe++;
			else if(strcmp(resultTokens.parseTokens[0], "exit") == 0)
			{
				isExit = 1;
				break;
			}
			else if(strcmp(resultTokens.parseTokens[0], "echo") == 0)
			{
				isEcho = 1;
				break;
			}
			else if(strcmp(resultTokens.parseTokens[0], "cd") == 0)
			{
				isCd = 1;
				break;
			}
			else if(strcmp(resultTokens.parseTokens[0], "io") == 0)
			{
				isIo = 1;
				break;
			}
			else
				continue;
		}
		
        if(isExit == 1)
        {
			// Make sure all of the background processes have terminated.
			while(next != 0)
			{
				// There are still processes in the queue.
				// Wait for the processes to finish executing.
				waitpid(queue[0].pid, NULL, 0);
				finish(queue, next);
				next--;
			}
            exitShell(beginTime);
        }
        else if(isEcho == 1)
        {
            echoShell(resultTokens.parseTokens, resultTokens.tokenAmount);
        }
        else if(isCd == 1)
        {
            
            if (resultTokens.tokenAmount > 2)
            {
                printf("Error: Too many arguments\n");
            }
           	else if (resultTokens.tokenAmount < 1)
            {
				chdir(getenv("HOME"));
            } 
            else {
                cdShell(resultTokens.parseTokens[1]);
            }
        }
		else if(isIo == 1)
		{
			
		}
		else if(isAmp != 0)
		{
			char** sendMe = mkAmends(resultTokens.parseTokens, resultTokens.tokenAmount, isAmp);
			if(sendMe == NULL)
			{
				continue;
			}
			int amount = resultTokens.tokenAmount - 1;
			send2back(sendMe, queue, amount, imHigh, next);
			next++;
			imHigh++;			
		}
		else if(isDire != 0)
		{
			parseIO(resultTokens);
		}
		else if(isPipe != 0)
		{
			parsePipe(resultTokens);
		}
		else
		{
			execute(resultTokens.parseTokens);
		}

		
		// Check to see if the next background process in the queue has terminated.
		if(next != 0)
		{
			ready = waitpid(queue[0].pid, NULL, WNOHANG);
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
