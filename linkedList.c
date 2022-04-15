#include"linkedList.h"

//Universally accessable which is bad, but we don't know better solutions (yet)
static struct PidNode* last = NULL;

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
