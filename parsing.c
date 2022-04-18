#include "common.h"
#include "parsing.h"
#include "utils.c"


int getArgLen(char **parsed) {

    int argLen = 0;

	//while((void*) parsed[++argLen] != NULL) {/*Do nothing ¯\_(ツ)_/¯*/}
    for(int i = 0; 1 ; i++)
    {
        if (((void*)parsed[i] == NULL)) {
            argLen = i;
            break;
        }
    }
    return argLen;
}

int freeArgs(char **parsed) {
	for(int i = getArgLen(parsed) -1; i >= 0; i--) {
        free(parsed[i]);
	}
    return 0;
}


char* parseMalloc(char *parsedString) {
    if (parsedString != NULL) {
        printf("Starting parseMalloc with string %s\n", parsedString);
        char temp[sizeof(parsedString)];
        strcpy(temp, parsedString);
        parsedString = (char*) malloc(sizeof(temp));
        strcpy(parsedString, temp);
    }
    return parsedString;
}

void parseCharToArgs(char **parsed, char splitter) {

	int argLen = getArgLen(parsed);

	for(int i = 0; i < argLen ; i++ ) {     
		//Do someting, checking all elements of arglist for the splitter symbol
        printf("Round %i, dealing with[%s]\n", i, parsed[i]);
        
		if (strlen(parsed[i]) > 1) { //if present:
                printf("Str: %s\n", parsed[i]);
            char *p_char = strchr(parsed[i], (int) splitter);
            if ( p_char != NULL) { //as long as it's more than one symbol
                for (int j = argLen -1 ; j > i ; j--) { //then move up all follwing indexes
                    parsed[j+1] = parsed[j];
                }
                argLen++;

                printf("Before checking first symbol\n");
                fflush(stdout);


			//then split
                printf("First symbol is: %c\n", parsed[i][0]);
                if (parsed[i][0] == splitter) { //case: first char is splitter
                    parsed[i+1] = (char*) (p_char +1);
                    parsed[i+1] = parseMalloc(parsed[i+1]);
                    char symbols[2] = {splitter, '\0'};

                    free(parsed[i]);
                    parsed[i] = parseMalloc(symbols);
                } else { //splitter is later in the string:
                    /*
                    */
                    char after[MAXCOM] = {0};
                    strcpy(after, parsed[i]);

                    char before[MAXCOM] = {0};
                    

                    strncpy(before, parsed[i], (int) (p_char -parsed[i]));
                    strcpy(after, p_char);
                    printf("Before: %s, After: %s\n", before, after);

                    free(parsed[i]);
                    parsed[i] = before;
                    parsed[i] = parseMalloc(parsed[i]);
                    parsed[i+1] = after;
                    parsed[i+1] = parseMalloc(parsed[i+1]);
/*
*/
                }
            }
		}
        checkArgsList(parsed);
        printf("End of round %i\n", i);
    }
}

// function for finding pipe
int parsePipe(char* str, char** strpiped)
{
	for (int i = 0; i < 2; i++) {
		strpiped[i] = strsep(&str, "|");
		if (strpiped[i] == NULL)
			break;
	}

	if (strpiped[1] == NULL)
		return 0; // returns zero if no pipe is found.
	else {
		return 1;
	}
}

// function for parsing command words
void parseSpace(char* str, char** parsed)
{
	for (int i = 0; i < MAXLIST; i++) {
        
		parsed[i] = strsep(&str, " ");
        parsed[i] = parseMalloc(parsed[i]);

		if (parsed[i] == NULL)
			break;
		if (strlen(parsed[i]) == 0)
			i--;
	}
}

/**
 * @brief Checks if the last non-space (' ') character is '&'
 * 
 * @param str The string to check
 * @return int true if last non-space character is '&'
 */
int parseDaemon(char *str) {

    int isDaemon = 0;

    for(int i = strlen(str)-1; i > 0; i--) {
        char end = str[i];
        if (end == '&' ){
                isDaemon = 1;
                str[i] = ' ';
                break;
        } else if (end == ' ') {
                continue;
        } else {
            break;
        }
    }

    return isDaemon;
}