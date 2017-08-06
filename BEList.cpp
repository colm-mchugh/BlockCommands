#include "BEList.h"
#include <stdlib.h>
#include <string.h>

BEList *createBEList() {
	BEList *new_list = (BEList*)malloc(sizeof(BEList));
	new_list->head = NULL;
	new_list->tail = NULL;
	new_list->num_nodes = 0;
	return new_list;
}

BEListNode *createBEListNode(BlockEntry* data) {
	BEListNode *new_node = (BEListNode*)malloc(sizeof(BEListNode));
	new_node->data = data;
	new_node->next = NULL;
	new_node->prev = NULL;
	return new_node;
}

// free memory allocated for the BEList
void deleteBEList(BEList** list_ptr, deleter delete_fn ) {
	if ((list_ptr != NULL) && (*list_ptr != NULL)) {
		BEList *list = *list_ptr;
		BEListNode *node = list->head;
		BEListNode *next_node = NULL;
		while(node != NULL) {
			// save the next node
			next_node = node->next;
			// hand off node's data to deleter function
			if (delete_fn != NULL) {
				delete_fn(node->data);
			}
			free(node);
			node = next_node;
		}
		free(list);
		*list_ptr = NULL;
	}
}

// Add data to the START of the list
void prependTo(BEList *list, BlockEntry *data) {
	BEListNode *new_node = createBEListNode(data);
	if (list->head == NULL) {
		list->tail = new_node;
	} else {
		new_node->next = list->head;
		list->head->prev = new_node;
	}
	list->head = new_node;
	list->num_nodes++;
}

BlockEntry *unlinkElement(BEList *list, BEListNode *element);

// Remove first node that contains the given data 
BlockEntry *removeIfExists(BEList* list, BlockEntry* data) {
	BEListNode *element = list->head;
	while ((element != NULL) && (element->data != data)) {
		element = element->next;
	}
	if (element != NULL) {
		data = unlinkElement(list, element);
	}
	return data;
}

// remove the given BEListNode from the list
// The BEListNode's memory is freed
// A pointer to the BlockEntry that was in the node is returned.
BlockEntry *unlinkElement(BEList *list, BEListNode *element) {
	if (element->prev == NULL) {
		list->head = element->next;
	} else {
		element->prev->next = element->next;
	}
	if (element->next == NULL) {
		list->tail = element->prev;
	} else {
		element->next->prev = element->prev;
	}
	BlockEntry *data = element->data;
	free(element);
	list->num_nodes--;
	return data;
}
		

