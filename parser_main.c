#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell_functions.h"

parseResult parse()
{
	char token[256]; // holds instruction token
	char **bucket;   // array that holds all instruction tokens
	char temp[256];  // used to split instruction tokens containing special characters

	////////////////////////////////
	char cwd[250];				  //
	char hostname[1024];		  //
	getcwd(cwd, 250);			  //
	hostname[1023] = '\0';		  //
	gethostname(hostname, 1023);  //
	printf("%s", getenv("USER")); // THIS IS THE PROMPT
	printf("@");				  //
	printf("%s", hostname);		  //
	printf(" :: ");				  //
	printf("%s", cwd);			  //
	printf(">");				  //
	////////////////////////////////

	int numI = 0; // number of tokens in an instruction

	do
	{ // loop reads character sequences separated by whitespace
		scanf("%s", token);

		int i;
		int start;

		start = 0;

		for (i = 0; i < strlen(token); i++)
		{

			if (token[i] == '|' || token[i] == '>' || token[i] == '<' || token[i] == '&')
			{

				// I/O redirection___________________________________________________________________________
				//int fd = open(path);

				if (i - start > 0)
				{
					memcpy(temp, token + start, i - start);
					temp[i - start] = '\0';
					bucket = addToken(bucket, temp, numI);
					numI++;
				}

				char specialChar[2];
				specialChar[0] = token[i];
				specialChar[1] = '\0';

				bucket = addToken(bucket, specialChar, numI);
				numI++;

				start = i + 1;
			}
		}
		if (start < strlen(token))
		{
			memcpy(temp, token + start, strlen(token) - start);
			temp[i - start] = '\0';
			bucket = addToken(bucket, temp, numI);
			numI++;
		}

		//PATH RESOLUTIOn-----------------------------------------------------
		//cd
		if (token[0] == 'c' && token[1] == 'd')
		{
			chdir(getenv("HOME"));
			getcwd(cwd, 250);
			printf("curent working directory: ");
			printf("%s", cwd);
			printf("\n");
			break;
		}

	} while ('\n' != getchar()); //until end of line is reached

	//RETURN STRUCT
	struct parseResult resultTokens = {bucket, numI};
	return resultTokens;
}

//reallocates instruction array to hold another token,
//returns new pointer to instruction array
char **addToken(char **instr, char *tok, int numTokens)
{
	int i;

	char **new_arr;
	new_arr = (char **)malloc((numTokens + 1) * sizeof(char *));

	//copy values into new array
	for (i = 0; i < numTokens; i++)
	{
		new_arr[i] = (char *)malloc((strlen(instr[i]) + 1) * sizeof(char));
		strcpy(new_arr[i], instr[i]);
	}

	//add new token
	new_arr[numTokens] = (char *)malloc((strlen(tok) + 1) * sizeof(char));
	strcpy(new_arr[numTokens], tok);

	if (numTokens > 0)
		free(instr);

	return new_arr;
}

void printTokens(char **instr, int numTokens)
{
	int i;
	printf("Tokens:\n");
	for (i = 0; i < numTokens; i++)
		printf("#%s#\n", instr[i]);
}
