#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <stdlib.h>



typedef struct StackNode{
	Item elem;
	struct StackNode *next;
}StackNode;

typedef StackNode TCelSt; // Redenumirea unui nod la notatiile din curs 
typedef StackNode *ACelSt; // Redenumirea unui pointer la nod la notatiile din curs

typedef struct Stack{
	StackNode* head;  // Varful stivei
	long size; // Numarul de elemente din stiva
}Stack;

typedef Stack TStiva; // Redenumirea stivei la notatiile din curs
typedef Stack *ASt;//  // Redenumirea unui pointer la stiva la notatiile din curs

Stack* createStack(void){
	// TODO: Cerinta 1

	ASt stack = NULL;
	stack = (ASt)malloc(sizeof(TStiva));
	stack->head = NULL;
	stack->size = 0l;

	return stack;
}

int isStackEmpty(Stack* stack){
	// TODO: Cerinta 1

	if(stack->head == NULL)
		return 1;

	return 0;
}

void push(Stack *stack, Item elem){
	// TODO: Cerinta 1

	ACelSt newCel = NULL;
	newCel = (ACelSt)malloc(sizeof(TCelSt));
	newCel->elem = elem;
	if(isStackEmpty(stack)) {
		stack->head = newCel;
		newCel->next = NULL;
	} else {
		newCel->next = stack->head;
		stack->head = newCel;
	}
	stack->size += 1l;


	return;
}

Item top(Stack *stack){	
	// TODO: Cerinta 1

	return stack->head->elem;

} 

void pop(Stack *stack){
	// TODO: Cerinta 1

	if(isStackEmpty(stack)) return;
	if(stack->head->next == NULL) {
		free(stack->head);
		stack->head = NULL;
	} else {
		ACelSt delete;
		delete = stack->head;
		stack->head = stack->head->next;
		free(delete);
	}
	stack->size -= 1;

	return;
}

void destroyStack(Stack *stack){
	// TODO: Cerinta 1

	ACelSt delete = NULL;
	while(stack->head != NULL) {
		delete = stack->head;
		stack->head = stack->head->next;
		free(delete);
	}
	free(stack);

}

#endif 
