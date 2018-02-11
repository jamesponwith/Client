/**
 *
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parse_args.h"

// Evil global
static char cmdline_copy[MAXLINE];

void parseArguments(const char *cmdline, char **argv) {
    unsigned int j;
    for(j = 0; j < sizeof(argv)-1; j++) {
        argv[j] = NULL;
    }

    strcpy(cmdline_copy, cmdline);

    char *token;
    char *remainder = cmdline_copy;
    char delim[] = " \n";
    unsigned int i = 0;

    while((token = strtok_r(remainder, delim, &remainder))) {
        argv[i] = token;
        i++;
    }
}
