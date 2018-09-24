#ifndef shell_functions_H_ 
#define shell_functions_H_
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
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
void execute(char **command);

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
void send2back(char** someToks, Process* queue, int howMany, int thisHigh, int index);
void start(int queNum, pid_t pid);
void finish(Process* queue, int next);
char** cleanToks(char **instr, int numToks, int where);
char** mkAmends(char** instr, int numToks, int numAmps);

// Initialize background process array.
Process* queue;
// Used to traverse the background process array.
int next, capacity, imHigh, ready;
// Counters for the special characters in a command.
int isAmp, isDire, isPipe, isExit, isEcho, isCd, isIo;

// I/O
parseResult parseIO(parseResult resultTokens);
void outRedirect(char* temp, char* temp2, char* temp3, parseResult resultTokens);
void inputRedirect(char* temp, char* temp3, parseResult resultTokens);

// Pipes
parseResult parsePipe(parseResult resultTokens);
void ourPipe(char* temp, char* temp2, parseResult resultTokens);

// Builtin functions
void exitShell(struct timeval beginTime);
void echoShell(char** args, int argsNum);
void cdShell(char* path);

// Environement variables: returns string with value of environment variable
// Parameter string should be in the format: $VARIABLE
parseResult parseEnv(parseResult resultTokens);
char* environmentVariable(char* var);

// Path resolution
char* prefixCommand(char* command);
char* resolvePath(char* pathToResolve);
char *strrev(char *str); //implementation of std <string.h> func from S/O
int isDirectory(char* path);

#endif