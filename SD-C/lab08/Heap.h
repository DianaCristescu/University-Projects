#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdlib.h>
#include <math.h>

/* We assume there is a defined primitive type Item. */
typedef struct {
	int prior; // Item priority
	Item data; // Item d
}ItemType;

typedef struct heap{
	long maxHeapSize; // capacity
	long size; // number of elements
	ItemType *elem; // array of elements
} PriQueue, *APriQueue;


PriQueue* makeQueue(int maxHeapSize){
	//TODO:

	PriQueue *queue = (PriQueue*)malloc(sizeof(PriQueue));
	queue->maxHeapSize = maxHeapSize;
	queue->size = 0;
	queue->elem = (ItemType*)malloc(maxHeapSize * sizeof(ItemType));

	return queue;
}

int getLeftChild(int i){
	//TODO:
	return (2*i + 1);
}

int getRightChild(int i) {
	//TODO:
	return (2*i + 2);
}

int getParent(int i) {
	//TODO:
	return ((i - 1)/2);
}

ItemType getMax(APriQueue h) {
	//TODO:
	return h->elem[0];
}

void siftUp(APriQueue h, int idx) {
	//TODO:

	if(h->size <= 1 || idx > h->size || idx < 1) return;

	int parent_idx = getParent(idx);
	if((h->elem[idx]).prior > (h->elem[parent_idx]).prior) {
		
		// interschimb elementul cu parintele sau
		ItemType parent = h->elem[parent_idx];
		h->elem[parent_idx] = h->elem[idx];
		h->elem[idx] = parent;
		siftUp(h, parent_idx);

	}

}


void insert(PriQueue *h, ItemType x) {
	//TODO:

	if (h->maxHeapSize <= (h->size + 1)) {
		h->maxHeapSize *= 2;
		h->elem = (ItemType*)realloc(h->elem, h->maxHeapSize*sizeof(ItemType));
	}

	h->elem[h->size] = x;
	h->size += 1;

	siftUp(h, h->size-1);

}

void siftDown(APriQueue h, int idx){
	//TODO:

	int max_idx = floor(log2(h->size));
	if(h->size <= 1 || idx >= max_idx || idx < 0) return;

	int left_child_idx = getLeftChild(idx);
	int right_child_idx = getRightChild(idx);
	if((h->elem[idx]).prior < (h->elem[left_child_idx]).prior) {
		
		// interschimb elementul cu copilul sau de stanga
		ItemType left_child = h->elem[left_child_idx];
		h->elem[left_child_idx] = h->elem[idx];
		h->elem[idx] = left_child;
		siftDown(h, left_child_idx);

	} else if((h->elem[idx]).prior < (h->elem[right_child_idx]).prior) {

		// interschimb elementul cu copilul sau de dreapta
		ItemType right_child = h->elem[right_child_idx];
		h->elem[right_child_idx] = h->elem[idx];
		h->elem[idx] = right_child;
		siftDown(h, right_child_idx);

	}

}

void removeMax(APriQueue h) {
	//TODO:

	h->elem[0] = h->elem[h->size-1];
	h->elem[h->size-1].prior = 0;
	h->elem[h->size-1].data = 0;
	h->size--;
	siftDown(h, 0);

}

void freeQueue(APriQueue h){
	// TODO:

	free(h->elem);
	free(h);

}

#endif
