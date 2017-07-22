/* 
 * File:   BEList.h
 * Author: colm_mchugh
 *
 * Created on July 13, 2017, 8:39 AM
 */

#ifndef BELIST_H
#define	BELIST_H

#include "commands_data.h"

typedef struct _node {
	BlockEntry *data;
	struct _node *next;
	struct _node *prev;
} BEListNode;

typedef struct _list {
	BEListNode *head;
	BEListNode *tail;
	int num_nodes;
} BEList;

#define list_size(l) (l->num_nodes)

typedef bool (*deleter)(BlockEntry*);

// Create an empty list
BEList *createBEList();

// Return the data that matches the given BlockEntry key (NULL if no match)
BlockEntry *getMatch(BEList *, char *);

// Remove the node that contains the given data (nop if not in list)
BlockEntry *removeIfExists(BEList*, BlockEntry*);

// Prepend the given entry to the start of the list
void prependTo(BEList*, BlockEntry*);

// Delete all nodes in a list, applying deleter function to each node's entry
void deleteBEList(BEList**, deleter);

#endif	/* BELIST_H */

