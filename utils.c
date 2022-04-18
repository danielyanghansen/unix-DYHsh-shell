#include"common.h"
#include"parsing.h"


void checkArgsList(char **parsedArgs) {
    //debugging
    for (int i = 0; i < getArgLen(parsedArgs); i++) {
        printf("Entry %i: [%s]\n", i, parsedArgs[i]);
    }
}
