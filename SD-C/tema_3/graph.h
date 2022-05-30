#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define MAX 256

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
	int n;
	ATNode *adl;
	char **name;
}	TGraphL;

void alloc_list(TGraphL *G, int n)
{
    G->nn = n;
	G->adl = (ATNode*)malloc((n+1)*sizeof(ATNode));
	G->name = (char**)malloc((n+1)*sizeof(char*));
	for(int i = 0; i < n; i++)
	{
		G->adl[i] = NULL;
		G->name[i] = NULL;
	} 

}

void free_list(TGraphL *G)
{
    ATNode it;
    for(int i = 0; i < G->n; i++)
	{
		it = G->adl[i];
		while(it != NULL)
		{
			ATNode aux = it;
			it = it->next;
			free(aux);
		}
		free(G->name[i]);
		G->name[i] = NULL;
		G->adl[i] = NULL;
	}
	G->nn = 0;
}

TGraphL* alloc_graph(TGraphL **G, int n) 
{
    *G = (TGraphL*)malloc(sizeof(TGraphL));
	(*G)->n = 0;
    alloc_list(*G, n);
    return *G;
}

int find_idx(TGraphL *G, char *s) 
{
	for(int i=0; i<G->n; i++)
	{
		if(!strcmp(s, G->name[i])) return i;
	}
	return -1;
}

void insert_edge_list(TGraphL *G, char *s1, char *s2, int c)
{
	int idx1 = find_idx(G, s1);
	if(idx1 < 0) 
	{
		idx1 = G->n;
		G->n++;
		G->name[idx1] = (char*)malloc(MAX*sizeof(char));
		strcpy(G->name[idx1], s1);
	}
	int idx2 = find_idx(G, s2);
	if(idx2 < 0) 
	{
		idx2 = G->n;
		G->n++;
		G->name[idx2] = (char*)malloc(MAX*sizeof(char));
		strcpy(G->name[idx2], s2);
	}

	TNode *t;
	t=(ATNode)malloc(sizeof(TNode));
	t->v = idx2; t->c=c; t->next = G->adl[idx1]; G->adl[idx1]=t;
}

void remove_node(TGraphL *G, char *s)
//sterg nodul s din graf
{
	int idx = find_idx(G, s);
	for(int i=0; i<G->n; i++)
	{
		if(G->adl[i] != NULL)
		{
			if(i == idx) 
			{
				ATNode it = G->adl[i];
				while(it != NULL)
				{
					ATNode aux = it;
					it = it->next;
					free(aux);
				}
				G->adl[i] = NULL;
			}
			else 
			{
				ATNode it = G->adl[i];
				while(it->next != NULL)
				{
					if(it->next->v == idx)
					{
						ATNode next = it->next->next;
						free(it->next);
						it->next = next;
						break;
					}
					it = it->next;
				}
			}
		}
	}
	free(G->name[idx]);
	G->name[idx] = NULL;
}

void print_graph(TGraphL *G)
{
	for(int i=0; i<G->n; i++)
	{
		if(G->name[i] != NULL && G->adl[i] != NULL)
		{
			printf("%s : ", G->name[i]);
			for(ATNode n=G->adl[i]; n != NULL; n=n->next)
				printf("%s ", G->name[n->v]);
			printf("\n");
		}
	}
}

void free_graph(TGraphL *G) 
{
    free_list(G);
	free(G->adl);
	free(G->name);
    free(G);
}