#include"common.h"
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include"linkedList.h"
#include"linkedList.c"

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

void parseRandom(char* str, char** parsed, char* splitter)
{
	for (int i = 0; i < MAXLIST; i++) {
		parsed[i] = strsep(&str, splitter);

		if (parsed[i] == NULL)
			break;
		if (strlen(parsed[i]) == 0)
			i--;
	}
}

void outputList(char** list) {
    printf("============\tArgLen: %i\t============\n\n", getArgLen(list));

    for(int i = 0; i < getArgLen(list); i++) {
        printf("%s\n", list[i]);
    }
    printf("\n============\t\t============\n", getArgLen(list));
}

int main() {
    char **strList = (char**) malloc(sizeof(char**) * MAXLIST);
    memset(strList, 0, sizeof(strList));
    /*

    

    strList[0] = "Hello there son";
    strList[1] = "SoS";
    strList[2] = "BoS";
    strList[3] = "LoS";

    printf("%s\n", strList[0]);
    */

    char inputString[MAXCOM] = "ls> files.txt && grep .c< files.txt> result\0";

    //parseSpace(inputString, strList);

    printf("Input String: %s\n",inputString);
    char* symbol = " ";

    parseRandom(inputString, strList, symbol);
    printf("After parsing spaces...\n");
    outputList(strList);

    parseCharToArgs(strList, '>');
    printf("After parsing '>'...\n");
    outputList(strList);
    parseCharToArgs(strList, '<');
    printf("After parsing '<'...\n");
    outputList(strList);


}