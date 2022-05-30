#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "heap.h"

typedef int TCost;
#define INF 2147483647

typedef struct node
{
	int v;
	TCost c;
	struct node *next;
} TNode, *ATNode;

typedef struct
{
	int nn;
	ATNode *adl;
}	TGraphL;


void alloc_list(TGraphL * G, int n)
{
    int i;
    G->nn = n;
	G->adl = (ATNode*) malloc((n+1)*sizeof(ATNode));
	for(i = 0; i < n; i++)
		G->adl[i] = NULL;

}

void free_list(TGraphL *G)
{
    int i;
    ATNode it;
    for(i = 1; i < G->nn; i++){
		it = G->adl[i];
		while(it != NULL){
			ATNode aux = it;
			it = it->next;
			free(aux);
		}
		G->adl[i] = NULL;
	}
	G->nn = 0;
}

void insert_edge_list(TGraphL *G, int v1, int v2, int c)
{
 TNode *t;
 t=(ATNode)malloc(sizeof(TNode));
 t->v = v2; t->c=c; t->next = G->adl[v1]; G->adl[v1]=t;
 t=(ATNode)malloc(sizeof(TNode));
 t->v = v1;  t->c=c; t->next = G->adl[v2]; G->adl[v2]=t;
}


void dijkstra(TGraphL G, int s)
{
	MinHeap *heap = newQueue(100);
	int cost[100] = {0};
	for(int i=0; i<G.nn; i++){
		if(i == s)
			minHeapInsert(heap, i, 0);
		else
			minHeapInsert(heap, i, INF);
	}

	MinHeapNode *min = NULL;
	while(!isEmpty(heap)){
		min = removeMin(heap);
		cost[min->v] = min->d;

		for(ATNode t = G.adl[min->v]; t != NULL; t = t->next){
			if(isInMinHeap(heap, t->v) && (heap->elem[heap->pos[t->v]])->d > min->d + t->c){
				(heap->elem[heap->pos[t->v]])->d = min->d + t->c;
				(heap->elem[heap->pos[t->v]])->v = t->v;
				SiftUp(heap, (heap->elem[heap->pos[t->v]])->v, (heap->elem[heap->pos[t->v]])->d);
			}
		} 
	}

	printf("\n  Varf   Drum minim\n");
	for(int i=0; i<G.nn; i++) {
		printf("    %d        %d\n", i, cost[i]);
	}

	destroyHeap(heap);
}

void Prim(TGraphL G)
{
	MinHeap *heap = newQueue(100);
	int lines[100] = {0};
	for(int i=0; i<G.nn; i++){
		if(i == 0)
			minHeapInsert(heap, i, 0);
		else
			minHeapInsert(heap, i, INF);
	}

	MinHeapNode *min = NULL;
	int parent = 0; //{varf, cost}
	while(!isEmpty(heap)) {
		min = removeMin(heap);

		for(ATNode t = G.adl[min->v]; t != NULL; t = t->next) {
			if(t->c == min->d) parent = t->v;
			if(isInMinHeap(heap, t->v) && (heap->elem[heap->pos[t->v]])->d > t->c) {
				(heap->elem[heap->pos[t->v]])->d = t->c;
				(heap->elem[heap->pos[t->v]])->v = t->v;
				SiftUp(heap, (heap->elem[heap->pos[t->v]])->v, (heap->elem[heap->pos[t->v]])->d);
			}
		} 

		lines[min->v] = parent;
	}

	printf("\n Parinte    Varf\n");
	for(int i=1; i<G.nn; i++) {
		printf("    %d        %d\n", lines[i], i);
	}

	destroyHeap(heap);
}


int main()
{
    int i,v1,v2,c;
	int V,E;
	TGraphL G;
	ATNode t;
	freopen ("graph.in", "r", stdin);
	scanf ("%d %d", &V, &E);
	alloc_list(&G, V);

	for (i=1; i<=E; i++)
	{
		scanf("%d %d %d", &v1, &v2, &c);
	    insert_edge_list(&G, v1,v2, c);
	}

	for(i=0;i<G.nn;i++)
	{
    	printf("%d : ", i);
    	for(t = G.adl[i]; t != NULL; t = t->next)
   			 printf("%d(%d) ",t->v, t->c);
    	printf("\n");
	}
	dijkstra(G,0);
	Prim(G);

	free_list(&G);
	return 0;
}