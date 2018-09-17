#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser_functions.h"

int main() {
    //initialize screen + token holder
    parseResult resultTokens;
    system("clear");
    
    while (1) {
        resultTokens = parse();

        //This was just for testing
        //printTokens(resultTokens.parseTokens, resultTokens.tokenAmount);
        
        /*
        
        resultTokens.parseTokens = The returned array with all the tokens
        
        resultTokens.tokenAmount = The amount of tokens in the array,
        so use this is as the limit when traversing through the array 

        e.g.

        for (i = 0; i < tokenAmount; i++)
        printf("%s", parseTokens[i]);
        */
        break;




	
    }
    return 0;
}
