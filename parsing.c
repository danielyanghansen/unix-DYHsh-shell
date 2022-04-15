#include "common.h"

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

void parseCharToArgs(char **parsed, char splitter) {

	int argLen = getArgLen(parsed);

	for(int i = 0; i < argLen ;printf("ArgLen: %i\n", argLen) && i++ ) {     
		//Do someting, checking all elements of arglist for the splitter symbol

        printf("\nDealing with string: %s\n", parsed[i]);
        
        volatile char *copy = parsed[i];
        char *p_char = strchr(copy, (int) splitter);
		if ( p_char != NULL) { //if present:
            printf("i: %i\n", i);


            if (strlen(parsed[i]) > 1) { //as long as it's more than one symbol
                for (int j = argLen -1 ; j > i ; j--) { //then move up all follwing indexes
                    printf("Moving index %i to %i\n", j, j+1);
                    parsed[j+1] = parsed[j];
                }
                argLen++;


			//then split
                if (parsed[i][0] == splitter) { //case: first char is splitter
                    parsed[i+1] = p_char +1;
                    char symbols[2] = {splitter, '\0'};
                    printf("Strlen symbol: %i", strlen(symbols));
                    parsed[i] = symbols;
                } else { //splitter is later in the string:
                    /*
                    */
                    char* after[MAXCOM] = {0};
                    strcpy(after, parsed[i]);

                    char* before[MAXCOM] = {0};
                    

                    strncpy(before, &copy[0], p_char -&copy[0]);

                    parsed[i] = before;
                    parsed[i+1] = p_char;
                    printf("Before: %s\nAfter: %s\n", parsed[i], parsed[i+1]);
                }
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

// function for parsing command words
void parseSpace(char* str, char** parsed)
{
	for (int i = 0; i < MAXLIST; i++) {
		parsed[i] = strsep(&str, " ");

		if (parsed[i] == NULL)
			break;
		if (strlen(parsed[i]) == 0)
			i--;
	}
}

//Currently checks everywhere. TODO ?: Rewrite to only check final char
int parseDaemon(char *str, char** strdaemon)
{
	/*
	int point = strlen(str) - 3; //room for a space at the end
	point = (point >= 0) ? point : 0;
	printf("point: %s", &str[point]);

	 if (strcmp(&str[point], "&")) {

	} */

	for (int i = 0; i < 2	; i++) {
		strdaemon[i] = strsep(&str, "&"); //Finds the first ampersand and strips the rest of the string. 
		if(strdaemon[i] == NULL)
			break;
	}

	if (strdaemon[1] == NULL)
		return 0;
	else
		return 1;

}