#include"utils.h"

int getArgLen(char **parsed) {
    for(int i = 0; i < MAXCOM ; i++) {
        if (parsed[i][0] == '\0') {
            return i;
        }
    }
    return MAXCOM;
}

void checkArgsList(char **parsedArgs) {
    for (int i = 0; i < getArgLen(parsedArgs); i++) {
        printf("Entry %i: [%s] of strlen %ld\n", i, parsedArgs[i], strlen(parsedArgs[i]));
    }
}
