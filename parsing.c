#include "parsing.h"
#include "utils.c"

int freeArgs(char **parsed) { //Currently unused
	for(int i = MAXLIST -1; i >= 0; i--) {
        free(parsed[i]);
	}
    return 0;
}

/**
 * @brief mallocs a string of length MAXCOM
 * 
 * @param parsedString 
 * @return char* 
 */
char* parseMalloc(char *parsedString) {
    char* temp = malloc(sizeof(char) * MAXCOM);
    if (parsedString != NULL) {
        strcpy(temp, parsedString);
    }
    return temp;
}

void parseCharToArgs(char **parsed, char splitter) {

	int argLen = getArgLen(parsed);

	for(int i = 0; i < argLen ; i++ ) {     
		//Do someting, checking all elements of arglist for the splitter symbol
        if(parsed[i][0] == '\0') break;

		if (strlen(parsed[i]) > 1) { //as long as it's more than one char to check

            if(parsed[i][0] == splitter && parsed[i][1] == '\0') goto parseEnd;

            char *p_char = strchr(parsed[i], (int) splitter);
        

            if (p_char != NULL) {
                for (int j = argLen -1 ; j > i ; j--) { //then move up all follwing indexes (except the last)
                    memcpy(parsed[j+1], parsed[j], sizeof(char *));
                }
            argLen++;

			//then split
                if (parsed[i][0] == splitter) { //case: first char is splitter

                    char symbols[2] = {splitter, '\0'};
                    strcpy(parsed[i+1], &parsed[i][1]);

                    strcpy(parsed[i], symbols);


                } else { //splitter is later in the string:
                    strcpy(parsed[i+1], p_char);
                    *p_char = '\0';
                }
            parseEnd:
                if(parsed[i +1][0] == '\0') break;
            }
		}
    }
}

// function for finding pipe
int parsePipe(char* str, char** strpiped) {
	for (int i = 0; i < MAXPIPE; i++) {
		strpiped[i] = strsep(&str, "|");
		if (strpiped[i][0] == '\0')
			break;
	}

	if (strpiped[1][0] == '\0')
		return 0; // returns zero if no pipe is found.
	else {
		return 1;
	}
}

int parseChar(char* str, char** parsed, char* splitter, int max) {
    char* token;

    token = strtok(str, splitter);   
	for (int i = 0; i < max; i++) {
		if (token == NULL) {
			break;
        }
        strcpy(parsed[i], token);
		if (strlen(parsed[i]) == 0) {
            i--;
        }
        token = strtok(NULL, splitter);
	}
    if (parsed[1][0] == '\0')
		return 0; // returns zero if no pipe is found.
	else {
		return 1;
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

void parseIO(char **parsed) {
	parseCharToArgs(parsed, '<');
	parseCharToArgs(parsed, '>');
	return;
}