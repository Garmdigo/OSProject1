#include "shell_functions.h"

//execute
void execute(char **command)
{
    int status;
    pid_t pid = fork();
    if (pid == -1)
        exit(1);
    else if (pid == 0)
    {
       if( execv(command[0], NULL) == -1){
        // printf("excv failed: %s\n", command[0]);  
	
	}
	exit(1);
    }
    else
        waitpid(pid, &status, 0);
}

// Starts new background process.
void send2back(char **someToks, Process queue[], int howMany, int thisHigh, int index)
{
    // Create  a new child process.
    pid_t aPid = fork();

    if (aPid == -1)
    {
        // Error, fork() failed. Exit the program.
        exit(1);
    }
    else if (aPid == 0)
    {
        // Child

        // Execute the command.
        execv(someToks[0], someToks);
		
        // Execv failed. Notify the user and exit.
        printf("Problem executing.\n");
        exit(1);
    }
    else
    {
        // Parent

        // Store the new background process in the queue.
        queue[index].theToks = someToks;
        queue[index].numToks = howMany;
        queue[index].pid = aPid;
        queue[index].spot = thisHigh;

        // Print out start message.
        start(queue[index].spot, queue[index].pid);

        return;
    }
}

parseResult parseIO(parseResult resultTokens)
{
	for(int i = 0; i<resultTokens.tokenAmount;i++)
	{
		char *temp;
		char *temp2;
		char *temp3;

		temp = resultTokens.parseTokens[i];
		
		//if redirection is first token, error
		if(i==0)
		{					
			if(strpbrk(resultTokens.parseTokens[0], "<") != NULL)
			{
				printf("I/O Error\n");
				return resultTokens;
			}
			else if(strpbrk(resultTokens.parseTokens[0], ">") != NULL)
			{
				printf("I/O Error\n");
				return resultTokens;
			}
		}

		//if redirection is last token, error
		if(i == (resultTokens.tokenAmount) - 1)
		{		
			for(int j = 0; j < strlen(temp); j++)
			{
				if(temp[j] == '<' || temp[j] == '>')
				{
					printf("I/O Error\n");
					return resultTokens;
				}
			}
		}

        for(int k = 0; k < strlen(temp);k++)
		{
			//checks for redirection token
			if(temp[k] == '>')
			{			
				temp2 = resultTokens.parseTokens[i-1];
				temp3 = resultTokens.parseTokens[i+1];
				outRedirect(temp, temp2, temp3, resultTokens);
			}
			else if(temp[k] == '<')
			{
				temp3 = resultTokens.parseTokens[i+1];
				inputRedirect(temp,temp3,resultTokens);
			}
		}
	}
	return resultTokens;
}

void outRedirect(char* temp, char* temp2, char* temp3, parseResult resultTokens){
   //printf("> spec found!\n");

    if(fork() == 0){
	//child
	open(temp3, O_RDWR | O_CREAT | O_TRUNC);
	close(1);
	dup(3);
	close(3);
	execute(resultTokens.parseTokens);
	exit(1);
   }
   else{
	close(3);
   }
   return;
}

void inputRedirect(char* temp, char* temp3, parseResult resultTokens){
   //printf("< spec found!\n");
	
   if(fork() == 0){
      //child
      open(temp3, O_RDONLY);
      close(0);
      dup(3);
      close(3);
      execute(resultTokens.parseTokens);
      exit(1);
   }
   else
      close(3);
return;

}

parseResult parsePipe(parseResult resultTokens)
{

	//Error Testing
   
	char* temp;
	char* temp2;
	
	for(int i = 0;i<resultTokens.tokenAmount;i++)
	{
		if(i==0)
		{
			if(strpbrk(resultTokens.parseTokens[0], "|") != NULL)
			{
				printf("Pipe Error\n");
				return resultTokens;
			} 
		}
		else if(i == 0 && resultTokens.tokenAmount == 1)
		{
			printf("HERE\n");
			return resultTokens;
		}
		else if(i==(resultTokens.tokenAmount) - 1 && i!=0)
		{
			if(strpbrk(resultTokens.parseTokens[i], "|") !=NULL)
			{
				printf("Pipe Error\n");
				return resultTokens;
			}
		}
	}

	temp = resultTokens.parseTokens[0];
	temp2 = resultTokens.parseTokens[2];
	if(resultTokens.tokenAmount != 1 && resultTokens.tokenAmount != 0)
      ourPipe(temp,temp2,resultTokens);

	return resultTokens;

}

void ourPipe(char* temp, char* temp2, parseResult resultTokens)
{

	int pipe(int pipefd[2]);
	int fd[2];

	if(fork()==0)
	{
		pipe(fd);
		if(fork()==0)
		{
			close(1);
			dup(fd[1]);
			close(fd[0]);
			close(fd[1]);
			execute(resultTokens.parseTokens);
			exit(1); 
		}
		else
		{
			close(0);
			dup(fd[0]);
			close(fd[0]);
			close(fd[1]);
			execute(resultTokens.parseTokens);
			exit(1);
		} 
	}
	else
	{
		close(fd[0]);
		close(fd[1]);
	}

}

// Helper for send2back. Print start message.
void start(int queNum, pid_t pid)
{
    printf("[%d]\t[%d]\n", queNum, pid);
}

// Removes background process from the queue and prints completion message.
void finish(Process* queue, int next)
{
	// Store the comand with the /bin/ prefix in cmd.
	char* cmd = (char*)malloc((strlen(queue[0].theToks[0])-4) * sizeof(char));
	strcpy(cmd, &queue[0].theToks[0][5]);
	
	// Print out the queue number.
	printf("[%d]+\t[", queue[0].spot);
	// Print out the command.
	printf("%s ", cmd);
	// Print out the arguments.
	for(int i = 1; i < queue[0].numToks - 1; i++)
	{
		printf("%s ", queue[0].theToks[i]);
	}
	printf("%s]\n", queue[0].theToks[queue[0].numToks-1]);
	 
	//Remove the process from the queue and move up the other processes.
	for(int i = 0; i < (next-1); i++)
	{
		queue[i].theToks = queue[i+1].theToks;
        queue[i].numToks = queue[i+1].numToks;
        queue[i].pid = queue[i+1].pid;
        queue[i].spot = queue[i+1].spot;
	}
}

// Helper for send2back. Returns an exact copy of the instr array.
char **copyToks(char **instr, int numToks)
{
    char **new_arr = (char **)malloc((numToks) * sizeof(char *));

    //copy values into new array
    for (int i = 0; i < numToks; i++)
    {
        new_arr[i] = (char *)malloc((strlen(instr[i]) + 1) * sizeof(char));
        strcpy(new_arr[i], instr[i]);
    }

    return new_arr;
}

//environment variables
parseResult parseEnv(parseResult resultTokens)
{
    for (int i = 0; i < resultTokens.tokenAmount; i++)
    {
        char *test;
        test = resultTokens.parseTokens[i];
        if (test[0] == '$')
        {
            resultTokens.parseTokens[i] = environmentVariable(resultTokens.parseTokens[i]);
        }
    }
    return resultTokens;
}

//parameter string should be in the format: $VARIABLE (though lowercase is accepted)
//environment variable: Returns string with value of environemnt variable
char *environmentVariable(char *var)
{
    char capsVar[strlen(var)];
    //starts loop at [1] to disregard '$' changes var to uppercase
    for (int i = 1; i <= strlen(var); i++)
    {

        capsVar[i - 1] = toupper(var[i]);
    }
    //testing value of capsvar: printf("\n%s\n", capsVar);
    return getenv(capsVar);
}

//path resolution: Four functions
char* prefixCommand(char* command)
{
    int builtinCD = strcmp(command, "cd");
    int builtinIO = strcmp(command, "io");
    int builtinECHO = strcmp(command, "echo");
    int builtinEXIT = strcmp(command, "exit");
    
    if (builtinCD != 0 && builtinIO != 0 && builtinECHO != 0 && builtinEXIT != 0)
    {
	//CODY---------------------------------------------------------------------------------
		
		// char* prefix = getenv("PWD");	


	//-----------------------------------------------------------------------------------------

        char *prefix = "/bin/";
        size_t length = strlen(command) + strlen(prefix) + 1;
        char *exCommand = malloc(sizeof(char) * length);
        strcpy(exCommand, prefix);

		// exCommand[(strlen(prefix))] = '/';

        strcat(exCommand, command);
        command = exCommand;
    }

return command;
}
char *resolvePath(char *pathToResolve)
{
    char *prefix;
    char *command = pathToResolve;
    char *exCommand;
    size_t length;

    //DIRNAME
    //int dirCheck = strcmp(command[0], "/");
    if (command[0] != '/' && command[0] != '.' && command[0] != '~')
    {
        prefix = getenv("PWD");

        length = strlen(command) + strlen(prefix) + 1;
        exCommand = malloc(sizeof(char) * length);
        strcpy(exCommand, prefix);

        exCommand[(strlen(prefix))] = '/';

        strcat(exCommand, command);
        command = exCommand;
    }

    if (command[0] == '~')
    {
        prefix = getenv("HOME");

        length = strlen(command) + strlen(prefix) + 1;
        exCommand = malloc(sizeof(char) * length);
        //char* temp = malloc(sizeof(char) * strlen(command));
        
        command++;
        strcpy(exCommand, prefix);

        exCommand[(strlen(prefix))] = '/';

        strcat(exCommand, command);
        command = exCommand; 
    }

    if (command[0] == '.' && command[1] != '.')
    {
        prefix = getenv("HOME");

        length = strlen(command) + strlen(prefix) + 1;
        exCommand = malloc(sizeof(char) * length);
        command++;
        command++;
        strcpy(exCommand, prefix);

        exCommand[(strlen(prefix))] = '/';

        strcat(exCommand, command);
        command = exCommand;
    }

    if (command[0] == '.' && command[1] == '.' && command[2] == '/')
    {
        //reverse string
        prefix = getenv("PWD");
        strrev(prefix);
        //delete up to previous directory using pointer++
        char i;
        int count = 0;
        while (1)
        {
            if (prefix[0] == '/')
            {
                prefix++;
                break;
            }
            else {
                prefix++;
                count++;
            }
        } 
        //moving command
        command++;
        command++;
        command++;

        strrev(prefix);
        length = strlen(command) + strlen(prefix) + 1;
        exCommand = malloc(sizeof(char) * length);
        strcpy(exCommand, prefix);

        exCommand[(strlen(prefix))] = '/';

        strcat(exCommand, command);
        command = exCommand;
    }

    return command;
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
    exit(0);
}

//echo: needs the char** of tokens, and the size of the char**
void echoShell(char **args, int argsNum)
{
    //using char* to get the full token
    char *test;

    //loops through all provided tokens
    for (int i = 1; i < argsNum; i++)
    {
        test = args[i];
        if (test[0] == '$')
        {
            printf("%s ", environmentVariable(test));
        }
        else
        {
            printf("%s ", test);
        }
    }
    printf("\n");
}

void cdShell(char *path)
{
    char directory[255];

    printf("%s\n", path);
    path = resolvePath(path);
     printf("%s\n", path);
    
    if(chdir(path) != 0) {
        printf("Error: %s does not exist!\n", directory);}

}

char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}