# OSProject1
COP4610 Project 1 Repository

Member 1: Cody O'Connor 

Member 2: Christian Watkins Parts 1,2,3,4,9

Member 3: John Beckmann, Background processes and main Shell loop



p1-ccj.tar contents:
- README
- makefile
- parser_help.c
- parser_main.c
- shell_functions.c
- shell_functions.h
- shell_main.c


To build:

$> make


To clean:

$> make clean

Known Bugs:
- cd without any arguments segfaults
- For background processes it can't handle ampersands that come in the beginning of the instruction and seg faults.
- Background processes can't handle all commands because path resolution doesn't search through $PATH. Does handle /bin/ instructions.

Completed using linprog


- Specific Parts:

  - Part 1: Parsing

    - Modularized The TA's code

  - Part 2: Environment variables

    - parseResult parseEnv(parseResult resultTokens). Function takes a parseReult input and tests for environment variables. If an environment variable is found then the environmentVariable() function is called to resolve it. The final resolved string is then returned to the main shell.
    
    - char* environmentVariable(char* var). The function for returning an environment variable takes an input of a char* in the form '$VARIABLE' (though lower case is accepted). It removes the '$' and uses the getenv() function to return a char* string containing the value of the environment variable

  - Part 3: Prompt

    - Integrated into the modified TA parser file. Uses getcwd(), gethost(), and getenv() to print out a prompt following the assignment format requirements
  
  - Part 4: Path resolution

    - char* prefixCommand(char* command). Every input parsed has its first token sent to this function. If the command in the first token is not builtin then it has /bin/ attached to its front.
    - resolvePath function - works with prefix command to resolve the path.

  - Part 5: Execution

    - Shell can execute commands. When it is done with the given process, returns to shell.

  - Part 9: Builtins

    - Exit

      - void exitShell(struct timeval beginTime). Function is called from the main shell loop and takes in a timeval struct. Uses the supplied timeval struct and another generated on function call to calculate the run time of the shell and then exits with Exit(0).
    
    - Echo

      - void echoShell(char** args, int argsNum). Takes char** of tokens, and the size of the char**. Iterates through token list, check if each token is an environment variable. If the token is an environment variable, environmentVariable() is called and its value is printed. If the token is not an environment variable then no change is made and it is printed normally.
    
    - CD
      
      - void cdShell(char *path). Takes a path and attempts to change to it. If the path is not a directory or if it doesn't exist an error will be thrown

      - char *resolvePath(char *pathToResolve). Takes a char * path and will attempt to resolve it to an absolute path.

      - char *strrev(char *str). Reverses a string, is just an implementation of the strrev found in string.h

      - int isDirectory(char* path). Uses stat to gather data and then the S_ISDIR macro to determine if the directory is a directory

     - No Io command***
