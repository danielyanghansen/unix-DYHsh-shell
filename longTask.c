#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<time.h>


int main() {

    for(int i = 30; i >= 0; i--) {
        printf("Counting down: %i\n", i);
        fflush(stdout);
        sleep(1);
        }
    return 0;
}