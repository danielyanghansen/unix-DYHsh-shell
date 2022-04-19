#include"processLinkedList.h"

//Universally accessable which is bad, but we don't know better solutions (yet)

node_t * createNode(int pidData, char* str){
    
    node_t *node = (node_t*)malloc(sizeof(node_t));
    node->pidData = pidData;
    node->next = NULL;
    node->previous = NULL;
    strcpy(node->commandData, str);

    return node;
}

int setLastNode(node_t *node) {
    last = node;
    return 0;
}

int addNode(node_t *node) {  
    last->next = node;
    node->previous = last;
    node->next = NULL;

    last = node;
    
    return 0;
}           

int insertNode(node_t *node, node_t *prevNode) {
    node->next = prevNode->next;
    node->previous = prevNode;

    prevNode->next = node;
    node->next->previous = node;

    if (node->next == NULL) {
        last = node;
    }

    return 0;

}

int deleteNode(node_t *node) {
    if (node->previous != NULL) {
        node->previous->next = node->next;
    }

    if (node->next != NULL) {
        node->next->previous = node->previous;
    }        
    else {
        last = node->previous;
    }

    free(node);
    return 0;
}

void zombie_cleanup(node_t *start) {
	for(node_t* n = start->next; n != NULL; n = n->next){
		int wstatus;
		pid_t deadstatus = waitpid(n->pidData, &wstatus, WNOHANG | WUNTRACED | WCONTINUED);

		if (deadstatus != 0) {
			if (deadstatus > 0) {
				kill(deadstatus, SIGKILL);
			}

			printf("Exit status [%s] = %i\n", n->commandData, WEXITSTATUS(wstatus) );
			n = n->previous;
			deleteNode(n->next);
		}	
	}
	return;
}

void jobs(node_t* startNode) {
    printf("=============Processes: =============\n\n");
    node_t *n;
    n = startNode;

    printf("Parent PID: %i\n", n->pidData);

    while (n->next != NULL) { 
        n = n->next;
        printf("\tProcess PID: %i, Command: %s\n", n->pidData, n->commandData);
    }

    printf("=====================================\n");
    fflush(stdout);

    return;
}
