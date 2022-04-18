#ifndef __parsing_h__
#define __parsing_h__

int getArgLen(char **parsed);

int freeArgs(char **parsed);

char* parseMalloc(char *parsedString);
#endif