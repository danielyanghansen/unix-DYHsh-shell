#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<time.h>


int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Not enough args\n");
        printf("Needs 1 arguments: secondCount\n");
        return 0;
    }

    for(int i = atoi(argv[1]); i >= 0; i--) {
        printf("Counting down: %i\n", i);
        fflush(stdout);
        sleep(1);
        }
    
    char stuff[2];
    stuff[1] = 'a';
    stuff[2] = '\0';
    stuff[10] = '1';

    char **strList[20];

    strList[0] = "Hello\0";
    strList[1] = "World! ablalblalba\0";

    strList[2] = strList[1];
    strList[1] = strList[0];

    strList[0] = strList[2];

    if(NULL) {
        printf("NULL is nonzero in an if");
    }

    for (int i = 0; i < 3; i++) {
        printf("%s\n", strList[i]);
    }

    int arr[10];
    for (int i = 0; i<10; i++) {
        arr[i] = i;
        printf("Index %i: %i\n", i, arr[i]);
    }
    for (int i = 9; i >= 0; i--) {
        arr[i+1] = arr[i];
    }
    for (int i = 0; i<10; i++) printf("Index %i: %i\n", i, arr[i]);

    char *string, *found;
    string = strdup("Hello there");
    found = strsep(&string, " ");

    printf("Part1: %s\n", found);
    printf("Part2: %s\n", string);


    
    return 0;
}