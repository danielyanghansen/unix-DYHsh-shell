#ifndef __linkedlist_h__
#define __linkedlist_h__

#include"common.h"

typedef struct PidNode {
    int pidData;
    char commandData[MAXCOM];
    struct PidNode* next;
    struct PidNode* previous;
} node_t;

static struct PidNode* last;

node_t * createNode(int pidData, char* str);

/**
 * @brief Adds a node to the end of the linked list
 * 
 *
 * 
 * @param node 
 * @return int 
 */
int addNode(node_t *node);


/**
 * @brief LEAVE IT. I LIKE IT. -daniel :)
 * 
 * @param node 
 * @param prevNode 
 * @return int 
 */
int insertNode(node_t *node, node_t *prevNode);

int deleteNode(node_t *node);

int setLastNode(node_t* node);

#endif