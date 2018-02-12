/**
 * Implementation file for parse_args library. 
 *
 * This class includes a function that takes in command line
 * arguments and parse them with one global variable. The 
 * function simply tokenizes the arguments and returns the 
 * tokens to argv.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parse_args.h"

// Evil global
static char cmdline_copy[MAXLINE];

/**
 * Parse the command line and build an array to hold 
 * the return message from the server.
 *
 * @param cmdline A buffer to store the message received 
 * from the server.
 * @param argv An array to store the the parsed message 
 * from the server.
 */
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
