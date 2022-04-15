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
    return 0;
}