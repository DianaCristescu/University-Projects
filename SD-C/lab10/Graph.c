#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Util.h"
#include "Graph.h"

TGraphL* createGraphAdjList(int numberOfNodes) {
	//TODO: 1
	TGraphL *graph_list = NULL;
	graph_list = (TGraphL*)malloc(sizeof(TGraphL));
	graph_list->nn = numberOfNodes;
	graph_list->adl = NULL;
	graph_list->adl = (ATNode*)malloc(numberOfNodes*sizeof(ATNode));
	for(int i=0; i<numberOfNodes; i++) {
		graph_list->adl[i] = NULL;
	}
	return graph_list;
}

void addEdgeList(TGraphL* graph, int v1, int v2) {
	//TODO: 1
	ATNode node_v1 = (ATNode)malloc(sizeof(TNode));
	node_v1->v = v1;
	node_v1->c = 1; 
	node_v1->next = graph->adl[v2];
	graph->adl[v2] = node_v1;

	ATNode node_v2 = (ATNode)malloc(sizeof(TNode));
	node_v2->v = v2;
	node_v2->c = 1; 
	node_v2->next = graph->adl[v1];
	graph->adl[v1] = node_v2;
}

List* dfsIterative(TGraphL* graph, int s) {

	//TODO: 2
	int *visited = (int*)malloc(graph->nn*sizeof(int));
	for(int i=0; i<graph->nn; i++)
		visited[i] = 0;
	Stack *stack = createStack();
	List *list = createList();
	int el = -1; 

	push(stack, s);
	visited[s] = 1;
	while(!isStackEmpty(stack)) {
		el = top(stack);
		pop(stack);
		enqueue(list, el);

		ATNode el_list = graph->adl[el];
		while(el_list != NULL) {
			if(visited[el_list->v] == 0) {
				push(stack, el_list->v);
				visited[el_list->v] = 1;
			}
			el_list = el_list->next;
		}
	}

	free(visited);
	destroyStack(stack);
	return list;

}

void dfsRecHelper(TGraphL* graph, int* visited, List* path, int s) {
	//TODO: 3
	enqueue(path, s);
	visited[s] = 1;
	ATNode list = graph->adl[s];
	while(list != NULL) {
		if(visited[list->v] == 0)
			dfsRecHelper(graph, visited, path, list->v);
		list = list->next;
	}

}

List* dfsRecursive(TGraphL* graph, int s) {
	//TODO: 3
	int *visited = (int*)malloc(graph->nn*sizeof(int));
	for(int i=0; i<graph->nn; i++)
		visited[i] = 0;
	List *list = createList();
	dfsRecHelper(graph, visited, list, s);
	free(visited);
	return list;
}

List* bfs(TGraphL* graph, int s){
	// TODO: 4
	int *visited = (int*)malloc(graph->nn*sizeof(int));
	for(int i=0; i<graph->nn; i++)
		visited[i] = 0;
	List *list = createList();
	Queue *queue = createQueue();
	int el = -1; 

	enqueue(queue, s);
	visited[s] = 1;
	
	while(!isQueueEmpty(queue)) {
		el = front(queue);
		dequeue(queue);
		enqueue(list, el);

		ATNode el_list = graph->adl[el];
		while(el_list != NULL) {
			if(visited[el_list->v] == 0) {
				enqueue(queue, el_list->v);
				visited[el_list->v] = 1;
			}
			el_list = el_list->next;
		}
	}

	free(visited);
	destroyQueue(queue);
	return list;	
}


void destroyGraphAdjList(TGraphL* graph){
	// TODO: 5
	for(int i=0; i<graph->nn; i++)
		removeNodeList(graph, i);
	free(graph->adl);
	free(graph);
}

void removeEdgeList(TGraphL* graph, int v1, int v2){
	TNode* it = graph->adl[v1];
	TNode* prev = NULL;
	while(it != NULL && it->v != v2){
		prev = it;
		it = it->next;
	}

	if(it == NULL)return;

	if(prev != NULL)
		prev->next = it->next;
	else
		graph->adl[v1] = it->next;
	free(it);

	it = graph->adl[v2];
	prev = NULL;
	while(it != NULL && it->v != v1){
		prev = it;
		it = it->next;
	}
	if(it == NULL) return;
	if(prev != NULL)
		prev->next = it->next;
	else
		graph->adl[v2] = it->next;

	free(it);
}

void removeNodeList(TGraphL* graph, int v){
	for(int i = 0; i < graph->nn;i++){
		removeEdgeList(graph,v,i);
	}
}
