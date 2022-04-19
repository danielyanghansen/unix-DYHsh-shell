#include"utils.h"

int getArgLen(char **parsed) {
    int argLen = 0;

    for(int i = 0; 1 ; i++) {
        if ( (void*) parsed[i] == NULL) {
            argLen = i;
            break;
        }
    }
    return argLen;
}

void checkArgsList(char **parsedArgs) {
    for (int i = 0; i < getArgLen(parsedArgs); i++) {
        printf("Entry %i: [%s] of strlen %ld\n", i, parsedArgs[i], strlen(parsedArgs[i]));
    }
}
