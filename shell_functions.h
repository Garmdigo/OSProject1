#ifndef shell_functions_H_ 
#define shell_functions_H_
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>

//parser functions
typedef struct parseResult {
    char** parseTokens;
    int tokenAmount;
} parseResult;

//execution
void execute(char **cmd);

parseResult parse();
char** addToken(char** instr, char* tok, int numTokens);
void printTokens(char** instr, int numTokens);


typedef struct Process
{
    char** theToks; // Instruction tokens
    int numToks;	// Number of tokens
	pid_t pid;		// Process ID
	int spot;		// Queue number
} Process;

// Background process functions.
void send2back(char** someToks, Process queue[], int howMany, int thisHigh, int index);
char** copyToks(char** instr, int numToks);
void start(int queNum, pid_t pid);

// Initialize background process array.
Process background[10];

// Used to traverse the background process array.
int next;
int imHigh;

//builtin functions
void exitShell(struct timeval beginTime);
void echoShell(char** args, int argsNum);
void cdShell(char* path);

//environement variables: returns string with value of environment variable
//parameter string should be in the format: $VARIABLE
parseResult parseEnv(parseResult resultTokens);
char* environmentVariable(char* var);

//path resolution
char* prefixCommand(char* command);
parseResult resolvePath(parseResult resultTokens);


#endif
