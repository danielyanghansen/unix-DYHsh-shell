#ifndef __process_linkedlist_h__
#define __process_linkedlist_h__

#include"common.h"

typedef struct PidNode {
    int pidData;
    char commandData[MAXCOM];
    struct PidNode* next;
    struct PidNode* previous;
} node_t;

static struct PidNode* last;

node_t* createNode(int pidData, char* str);

/**
 * @brief Adds a node to the end of the linked list
 * 
 *
 * 
 * @param node 
 * @return int 
 */
int addNode(node_t *node);

int insertNode(node_t *node, node_t *prevNode);

int deleteNode(node_t *node);

int setLastNode(node_t* node);

//Function to remove zombie processes and remove old indexes from process linkedlist
void zombie_cleanup(node_t *start);

void jobs(node_t* startNode);

#endif