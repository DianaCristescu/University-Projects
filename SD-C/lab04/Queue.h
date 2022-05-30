#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct QueueNode{
	Item elem;
	struct QueueNode *next;
}QueueNode;

typedef QueueNode TcelQ;
typedef QueueNode *ACelQ;

typedef struct Queue{
	QueueNode *front;
	QueueNode *rear;
	long size;
}Queue;

typedef Queue TCoada;
typedef Queue *AQ;


Queue* createQueue(void){
	// TODO: Cerinta 2

	AQ queue = NULL;
	queue = (AQ)malloc(sizeof(TCoada));
	queue->front = NULL;
	queue->rear = NULL;
	queue->size = 0l;

	return queue;
} 

int isQueueEmpty(Queue *q){
	// TODO: Cerinta 2

	if(q->front == NULL && q->rear == NULL)
		return 1;

	return 0;
}

void enqueue(Queue *q, Item elem){
	// TODO: Cerinta 2

	ACelQ newCel = NULL;
	newCel = (ACelQ)malloc(sizeof(TcelQ));
	newCel->elem = elem;
	newCel->next = NULL;
	
	if(isQueueEmpty(q)) {
		q->front = newCel;
		q->rear = newCel;
	} else {
		q->rear->next = newCel;
		q->rear = newCel;
	}
	q->size += 1l;

	return;
}

Item front(Queue* q){
	// TODO: Cerinta 2

	return q->front->elem;
}

void dequeue(Queue* q){
	// TODO: Cerinta 2

	if(isQueueEmpty(q)) return;

	ACelQ delete;
	if(q->front == q->rear) {
		delete = q->front;
		q->front = NULL;
		q->rear = NULL;
		free(delete);
	} else {
		delete = q->front;
		q->front = q->front->next;
		free(delete);
	}
	q->size -= 1l;

	return;
}

void destroyQueue(Queue *q){
	// TODO: Cerinta 2

	ACelQ delete = NULL;
	while(q->front != NULL) {
		delete = q->front;
		q->front = q->front->next;
		free(delete);
	}
	free(q);

	return;
}

#endif
