#include "parsing.h"

int freeArgs(char **parsed) { //Currently unused
	for(int i = getArgLen(parsed) -1; i >= 0; i--) {
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
    char* temp = malloc(sizeof(char)*MAXCOM);
    if (parsedString != NULL) {
        strcpy(temp, parsedString);
    }
    return temp;
}

void parseCharToArgs(char **parsed, char splitter) {

	int argLen = getArgLen(parsed);

	for(int i = 0; i < argLen ; i++ ) {     
		//Do someting, checking all elements of arglist for the splitter symbol
        if(parsed[i] == NULL) break;
		if (strlen(parsed[i]) > 1) { //if present:
            char *p_char = strchr(parsed[i], (int) splitter);
            int state = (p_char == NULL);

            if (state) {
                continue;
            }

            if (!state) { //as long as it's more than one symbol
                for (int j = argLen -1 ; j > i ; j--) { //then move up all follwing indexes (except the last)
                    parsed[j+1] = parsed[j];
                }
                argLen++;

			//then split
                if (parsed[i][0] == splitter) { //case: first char is splitter

                    char symbols[2] = {splitter, '\0'};
                    char temp[MAXCOM];

                    strcpy(temp, parsed[i]);
                    free(parsed[i]);
                    parsed[i] = parseMalloc(symbols);
                    char *p_char2 = (char*) (strchr(temp, (int) splitter) + 1);
                    parsed[i+1] = parseMalloc((char*) p_char2);

                } else { //splitter is later in the string:
                    char after[MAXCOM] = {0};
                    strcpy(after, parsed[i]); //copy over the whole string

                    char before[MAXCOM] = {0};
                    strncpy(before, parsed[i], (int) (p_char -parsed[i]));
                    strcpy(after, p_char);

                    free(parsed[i]);
                    parsed[i] = parseMalloc(before);
                    parsed[i+1] = parseMalloc(after);
                }
            if(parsed[i +1] == NULL) break;
            }
		}
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

void parseChar(char* str, char** parsed, char* splitter)
{
    char s[2];
    strcpy(s,splitter);
    char* token;

    token = strtok(str, s);   
	for (int i = 0; i < MAXLIST; i++) {
		if (token == NULL) {
			break;
        }
        parsed[i] = parseMalloc(token);
		if (strlen(parsed[i]) == 0) {
            i--;
        }
        token = strtok(NULL, s);
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