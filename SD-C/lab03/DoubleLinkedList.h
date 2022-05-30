#ifndef _DOUBLE_LINKED_LIST_H_
#define _DOUBLE_LINKED_LIST_H_
#include <stdlib.h>
/*
  IMPORTANT!

  As we stick to pure C, we cannot use templates. We will just asume
  some type T was previously defined.
*/

// -----------------------------------------------------------------------------
/**
 *  Linked list representation -- R2 Curs2 (Slide 8)
 */
typedef struct ListNode{
	Item elem; // Stored node value
	struct ListNode* next; // link to next node
	struct ListNode* prev; // link to prev node
} ListNode;

/**
 *  Double linked list representation -- desen Curs 3 (Slide 19)
 */
typedef struct List{
	ListNode* first; // link to the first node
	ListNode* last; // link to the last node
}List;
// -----------------------------------------------------------------------------

/**
 * Create a new node
 *  [input]: Item
 *  [output]: ListNode*
 */
ListNode *createNode(Item elem) {
	// TODO: Cerinta 1a

    ListNode* newList = NULL;
    newList = (ListNode*)malloc(sizeof(ListNode));
    newList->elem = elem;
    newList->next = NULL;
	newList->prev = NULL;

	return newList;
}

/**
 * Create a new (empty)  list  -- Silde-urile 13/14 Curs 2
 *  [input]: None
 *  [output]: List*
 */
List* createList(void){
	// TODO: Cerinta 1a

	List* newList = NULL;
	newList = (List*)malloc(sizeof(List));
	newList->first = NULL;
	newList->last = NULL;

	return newList;
}
// -----------------------------------------------------------------------------


/**
 * Determines if the list is empty
 *  [input]: List*
 *  [output]: 1 - empty/ 0 - not empty
 */
int isEmpty(List *list){
	// TODO: Cerinta 1b

	if(list->first == NULL && list->last == NULL)
		return 1;

	return 0;
}
// -----------------------------------------------------------------------------


/**
 * Determines if a list contains a specified element
 *  [input]: List*, Item
 *  [output]: int (1 - contains/ 0 - not contains)
 */
int contains(List *list, Item elem){
	// TDOO: Cerinta 1c

	ListNode* node = list->first;

	while(node != NULL) {
		if(node->elem == elem)
			return 1;
		node = node->next;
	}

	return 0;
}
// -----------------------------------------------------------------------------


/**
 * Compute list length
 *  [input]: List*
 *  [output]: int
 */
int length(List *list){
	// Guard against young player errors
	if(list == NULL) return 0;

	// TODO: Cerinta 1f

	int len = 0;
	ListNode* node = list->first;

	while(node != NULL) {
		len++;
		node = node->next;
	}

	return len;
}
// -----------------------------------------------------------------------------


/**
 * Insert a new element in the list at the specified position.
 * Note: Position numbering starts with the position at index zero
 *  [input]: List*, Item, int
 *  [output]: void
 */
void insertAt(List* list, Item elem, int pos){

	// Guard against young player errors
	if(list == NULL) return;

	// TODO: Cerinta 1d

	int len = length(list);
	if(pos < 0 || pos > len) return;

	ListNode* newNode = createNode(elem);
	if(newNode == NULL) return;

	if(len==0) {
		list->first = newNode;
		list->last = newNode;
	} else if(pos == 0) {
		newNode->next = list->first;
		list->first->prev = newNode;
		list->first = newNode;
	} else if(pos == len) {
		newNode->prev = list->last;
		list->last->next = newNode;
		list->last = newNode;
	} else {
		ListNode* node = list->first;
		for(int i=0; i<pos; i++)
			node = node->next;
		newNode->prev = node->prev;
		newNode->next = node;
		node->prev->next = newNode;
		node->prev = newNode;
	}
	return;

}
// -----------------------------------------------------------------------------


/**
 * Delete the first element instance form a list.
 *  [input]: List*, Item
 *  [output]: void
 */
void deleteOnce(List *list, Item elem){
	// Guard against young player errors
	if(list == NULL) return;

	//TODO: Cerinta 1e

	if(isEmpty(list) || !contains(list, elem)) return;

	ListNode* node = list->first;
	while(node != NULL) {
		if(node->elem == elem) {
			if(node->prev == NULL) {
				list->first = node->next;
				if(list->first != NULL)
					list->first->prev = NULL;
				else
					list->last = NULL;
				free(node);
				return;
			}
			if(node->next == NULL) {
				list->last = node->prev;
				list->last->next = NULL;
				free(node);
				return;
			}
			node->prev->next = node->next;
			node->next->prev = node->prev;
			free(node);
			return;
		}
		node = node->next;
	}

}
// -----------------------------------------------------------------------------



/**
 * Destroy a list.
 *  [input]: List*
 *  [output]: void
 */
List* destroyList(List* list){
	// Guard against young player errors
	if(list == NULL) return NULL;

	//TODO: Cerinta 1g

	ListNode* p1 = list->first;
	ListNode* p2 = list->last;
	ListNode* delete;

	while(p1 != p2 && p1->next != p2) {
		delete = p1;
		p1 = p1->next;
		free(delete);

		delete = p2;
		p2 = p2->prev;
		free(delete);
	}

	if(p1 == p2) {
		free(p1);
	} else {
		free(p1);
		free(p2);
	}


	free(list);
	list = NULL;

	return list;
}
// -----------------------------------------------------------------------------


#endif //_DOUBLE_LINKED_LIST_H_
