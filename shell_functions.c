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
         printf("excv failed: %s\n", command[0]);  
	
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

    // Store the full pathname of the command in com.
    char prefix[] = "/bin/";
    char *com = strcat(prefix, someToks[0]);

    if (aPid == -1)
    {
        // Error, fork() failed. Exit the program.
        exit(1);
    }
    else if (aPid == 0)
    {
        // Child

        // Execute the command.
        execv(com, someToks);

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
        queue[index].spot = thisHigh + 1;

        // Print out start message.
        start(queue[index].spot, queue[index].pid);

        return;
    }
}

parseResult parseIO(parseResult resultTokens){
   for(int i = 0; i<resultTokens.tokenAmount;i++){
      char *temp;
      char *temp2;
      char *temp3;

      temp = resultTokens.parseTokens[i];
      
      if(i==0){					//if redirection is first token, error
	 if(temp[5] == '>' || temp[5] == '<'){
	    printf("I/O Error\n");
	    return resultTokens;
 	 }
      }

      if(i == (resultTokens.tokenAmount) - 1){		//if redirection is last token, error
         for(int j = 0; j < strlen(temp); j++){
 	    if(temp[j] == '<' || temp[j] == '>'){
	       printf("I/O Error\n");
	       return resultTokens;
	    }
 	 }

      }


         for(int k = 0; k < strlen(temp);k++){
	    if(temp[k] == '>'){			//checks for redirection token
	       temp2 = resultTokens.parseTokens[i-1];
	       temp3 = resultTokens.parseTokens[i+1];
	       outRedirect(temp, temp2, temp3, resultTokens);
	    }
	    else if(temp[k] == '<'){
	        inputRedirect(temp,temp3);
		temp3 = resultTokens.parseTokens[i+1];
	    }
	 }
      
   }
   return resultTokens;
}

void outRedirect(char* temp, char* temp2, char* temp3, parseResult resultTokens){
   printf("> spec found!\n");

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
   return resultTokens;
}

void inputRedirect(char* temp, char* temp3){
   printf("< spec found!\n");
	



}

// Helper for send2back. Print start message.
void start(int queNum, pid_t pid)
{
    printf("[%d]\t[%d]\n", queNum, pid);
}

// Helper for send2back. Returns an exact copy of the instr array.
char **copyToks(char **instr, int numToks)
{
    int i;

    char **new_arr = (char **)malloc((numToks) * sizeof(char *));

    //copy values into new array
    for (i = 0; i < numToks; i++)
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
		
	char* prefix = getenv("PWD");	


	//-----------------------------------------------------------------------------------------

        //char *prefix = "/bin/";
        size_t length = strlen(command) + strlen(prefix) + 1;
        char *exCommand = malloc(sizeof(char) * length);
        strcpy(exCommand, prefix);

		exCommand[(strlen(prefix))] = '/';

        strcat(exCommand, command);
        command = exCommand;
    }

return command;
}
/*parseResult resolvePath(parseResult resultTokens)
{   
    parseResult resolvedTokens = resultTokens;
    int commandType = isCommand(resultTokens);
    
    //loops through all provided tokens
    for (int i = 0; i < resultTokens.tokenAmount; i++)
    {
        resultTokens.parseTokens[i] = expandPath(resultTokens.parseTokens[i], commandType);
    }
    return resolvedTokens;
}

//path resolution: Check for command type
int isCommand(parseResult resultTokens)
{

}

//path resolution: Expand the path
char* expandPath(char* path, int argType)
{

}
*/
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
    int i;
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
    //it goes into here
}
