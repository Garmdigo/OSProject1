#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser_functions.h"

int main() {
    char** arglist;
    system("clear");
    while (1) {
        arglist = parse();
        //printTokens(arglist, sizeof(arglist));
        break;
    }
    return 0;
}