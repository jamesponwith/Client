#ifndef __PARSE_ARGS_H__
#define __PARSE_ARGS_H__

#define MAXLINE     1024 // max command line size
#define MAXARGS     128

void parseArguments(const char *cmdline, char *argv[]);

#endif
