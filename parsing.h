#ifndef __parsing_h__
#define __parsing_h__

#include "common.h"

int freeArgs(char **parsed);

/**
 * @brief mallocs a string of length MAXCOM
 * 
 * @param parsedString 
 * @return char* 
 */
char* parseMalloc(char *parsedString);

void parseCharToArgs(char **parsed, char splitter);

int parseChar(char* str, char** parsed, char* splitter);

/**
 * @brief Checks if the last non-space (' ') character is '&'
 * 
 * @param str The string to check
 * @return int true if last non-space character is '&'
 */
int parseDaemon(char *str);

void parseIO(char **parsed);

#endif