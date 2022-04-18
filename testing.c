#include"common.h"
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include"linkedList.h"
#include"linkedList.c"
#include"parsing.c"

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
   char sym = '<';
   char symb[2] = {sym, '\0'};
   char **list[5] = {0};

   list[0] = symb;
   list[0] = parseMalloc(list[0]);
   printf("Symbol: %s, Length %i\n", list[0], strlen(list[0]));

   list[1] = list[0];
   list[0] = "Hello\n";
   list[0] = parseMalloc(list[0]);
   printf("Symbol: %s, Length %i\n", list[0], strlen(list[0]));
   printf("Symbol: %s, Length %i\n", list[1], strlen(list[1]));


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
