#include"linkedList.c"
#include<sys/types.h>

int main() {

    node_t *start = createNode(getpid());
    setLastNode(start);

    fflush(stdout);

    pid_t pid = fork();
	if (pid == -1) {
		printf("\nFailed forking child..");
		return -1;
	} else if (pid == 0) {
        for(int i = 2; i >= 0; i--) {
        printf("Counting down: %i\n", i);
        fflush(stdout);
        sleep(1);
        }

		exit(0);
	} else {
		node_t * node = createNode(pid);
       
		addNode(node); 
        node_t *n;
        n = start;

        printf("Parent PID: %i\n", n->pidData);
        printf("Child PID: %i\n", n->next->pidData);


        while (n->next != NULL) { 
            n = n->next;
            printf("Process PID: %i\n", n->pidData);
            fflush(stdout);
        }

		// waiting for child to terminate
		waitpid(pid, NULL, 0);
		return 0;
	}
}