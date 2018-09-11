#ifndef parser_functions_H_ 
#define parser_functions_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char** parse();
char** addToken(char** instr, char* tok, int numTokens);
void printTokens(char** instr, int numTokens);

#endif