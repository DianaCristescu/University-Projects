#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#define MAX 256

int cerinta1_recursiv(TGraphL *G, int i, int *visited)
//returneaza 1 daca graful e ciclic, 0 daca nu e
{
    if(G->adl[i] == NULL)
        return 0;
    else if(visited[i] == 1)
        return 1;
    else
    {    
        visited[i] = 1;
        for(ATNode n=G->adl[i]; n != NULL; n = n->next)
        {
            if(cerinta1_recursiv(G, n->v, visited)) return 1;
        }
        visited[i] = 0;
        return 0;
    }
}

char *cerinta1(TGraphL *G) 
//verifica daca graful orientat este ciclic sau nu cu ajutorul fct. recursive
{
    int visited[MAX] = {0};
    for(int i=0; i<G->n; i++) 
    {
        if(G->adl[i] != NULL)
        {
            if(cerinta1_recursiv(G, i, visited)) return "imposibil";
        }
    }

    return "corect";
}

int not_in_array(char **array, int len, char*s)
//returneaza 0 daca gaseste s in array
{
    for(int i=0; i<len; i++)
    {
        if(!strcmp(array[i], s)) return 0;
    }
    return 1;
}

TGraphL *pas1(TGraphL *G, char *x, char *y, char **o, int o_len)
{
    //fac vectorul de noduri pe care le pastrez
    int keep_len = o_len + 2;
    char **keep = NULL;
    keep = (char**)malloc(o_len*sizeof(char*));
    for(int i=0; i<keep_len; i++) keep[i] = NULL;
    keep[0] = (char*)malloc(MAX*sizeof(char));
    strcpy(keep[0], x);
    keep[1] = (char*)malloc(MAX*sizeof(char));
    strcpy(keep[1], y);
    for(int i=2; i<keep_len; i++)
    {
        keep[i] = (char*)malloc(MAX*sizeof(char));
        strcpy(keep[i], o[i-2]);
    }

    //scot toate nodurile care nu sunt in array si nu sunt parintii unor noduri din array
    int decide = 1;
    for(int i=0; i<G->n; i++)
    {
        if(G->adl[i] != NULL)
        {
            decide = 1;
            for(ATNode n=G->adl[i]; n!=NULL; n=n->next)
            {
                if(!not_in_array(keep, keep_len, G->name[n->v]))
                {
                    decide = 0;
                    break;
                }
            }
            if(not_in_array(keep, keep_len, G->name[i]) && decide)
                remove_node(G, G->name[i]);
        }
    }

    //dealoc vectorul facut
    for(int i=0; i<keep_len; i++)
        free(keep[i]);
    free(keep);
    return G;
}

int main(int argc, char* argv[]) 
{
    //pregatesc variabilele
    FILE *in, *out;
    char word[MAX];
    TGraphL *graph = NULL;
    graph = alloc_graph(&graph, MAX);

    if(strcmp(argv[1], "-c1") && strcmp(argv[1], "-c2")) 
    {
        in = fopen(argv[1], "r");
        out = fopen(argv[2], "w");
    }
    else 
    {
        in = fopen("bnet.in", "r");
        out = fopen("bnet.out", "w"); 
    }

    //citesc n, m
    fscanf(in, "%s ", word);
    int n = atoi(word);
    fscanf(in, "%s ", word);
    int m = atoi(word); 

    //citesc toate nodurile
    for(int i = 0; i<n; i++) 
    {
        fscanf(in, "%s ", word);
    }

    //Cerinta 1-------------------------------------------------------------------
    if(!strcmp(argv[1], "-c1"))
    {   
        //adaug laturile orientate in listele de adiacenta
        char **parents = (char**)malloc(MAX*sizeof(char*));
        char **children = (char**)malloc(MAX*sizeof(char*));
        int parents_len = 0;
        int children_len = 0;
        for(int i=0; i<MAX; i++)
        {
            parents[i] = NULL;
            children[i] = NULL;
        }

        char parent[MAX] = "";
        char child[MAX] = "";

        for(int i = 0; i<m; i++) 
        {        
            fscanf(in, "%s ", parent);
            fscanf(in, "%s ", child);
            insert_edge_list(graph, parent, child, 1);

            parents[i] = (char*)malloc(MAX*sizeof(char));
            parents_len++;
            strcpy(parents[i], parent);

            children[i] = (char*)malloc(MAX*sizeof(char));
            children_len++;
            strcpy(children[i], child);
        }

        //verific graful
        char result[MAX];
        strcpy(result, cerinta1(graph));
        fprintf(out, "%s\n", result);

        //transform graful orientat intr-un graf neorientat
        for(int i = 0; i<m; i++) 
        {
            insert_edge_list(graph, children[i], parents[i], 1);
        }

        //dealoc listele ajutatoare
        for(int i=0; i<parents_len; i++) free(parents[i]);
        for(int i=0; i<children_len; i++) free(children[i]);
        free(parents);
        free(children);
    }
    //Cerinta 2-------------------------------------------------------------------
    else if(strcmp(argv[1], "-c2"))
    {
        //adaug laturile orientate in listele de adiacenta
        char parent[MAX] = "";
        char child[MAX] = "";

        for(int i = 0; i<m; i++) 
        {        
            fscanf(in, "%s ", parent);
            fscanf(in, "%s ", child);
            insert_edge_list(graph, parent, child, 1);
        }

        //citesc k
        fscanf(in, "%s ", word);
        int k = atoi(word);

        //citesc k linii
        char **lines = (char**)malloc(MAX*sizeof(char*));
        for(int i=0; i<k; i++)
        {
            lines[i] = (char*)malloc(MAX*sizeof(char));
            fgets(lines[i], MAX-1, in);
        }

        //procesez k linii (scot x, y si lista O)
        char **x = (char**)malloc(MAX*sizeof(char*));
        char **y = (char**)malloc(MAX*sizeof(char*));
        char ***o = (char***)malloc(MAX*sizeof(char**));
        int o_len[MAX] = {0};
        int j = 0;
        int offset = 0;

        for(int i=0; i<k; i++)
        {
            //procesez x
            x[i] = (char*)malloc(MAX*sizeof(char));
            j = 0;
            while(lines[i][j] != ' ')
            {
                x[i][j] = lines[i][j];
                j++;
            }
            x[i][j+1] = '\0';

            //procesez y
            y[i] = (char*)malloc(MAX*sizeof(char));
            j += 3;
            offset = j;
            while(lines[i][j] != '|')
            {
                y[i][j-offset] = lines[i][j];
                j++;
            }
            y[i][j-offset+1] = '\0';

            //procesez O
            o[i] = (char**)malloc(MAX*sizeof(char*));
            offset = j+1;
            o[i][o_len[i]] = (char*)malloc(MAX*sizeof(char));
            while(lines[i][j] != '\0')
            {
                if(lines[i][j] != ' ' && lines[i][j] != '\n')
                    o[i][o_len[i]][j-offset] = lines[i][j];
                else
                {
                    o[i][o_len[i]][j-offset+1] = '\0';
                    o_len[i]++;
                    o[i][o_len[i]] = (char*)malloc(MAX*sizeof(char));
                    offset = j + 1;
                }
                j++;
            }
            o_len[i]++;
        }

        //dealocare lines
        for(int i=0; i<k; i++)
        {
            free(lines[i]);
        }
        free(lines);

        //algoritm-----------------------------------------------------
        TGraphL *g = NULL;
        for(int i=0; i<k; i++)
        {
            g = pas1(graph, x[i], y[i], o[i], o_len[i]);

            for(int i=0; i<g->n; i++)
            {
                if(g->name[i] != NULL && g->adl[i] != NULL)
                {
                    printf("%s : ", g->name[i]);
                    for(ATNode n=g->adl[i]; n != NULL; n=n->next)
                        printf("%s ", g->name[n->v]);
                    printf("\n");
                }
            }

            free(g);
        }

        // dealocare x, y, O
        for(int i=0; i<k; i++)
        {
            free(x[i]);
            free(y[i]);
            for(int j=0; j<o_len[i]; j++) free(o[i][j]);
            free(o[i]);
        }
        free(x);
        free(y);
        free(o);
    }

    //dealocari finale
    fclose(in);
    fclose(out);
    free_graph(graph);
    return 1;
}


        // //printez liniile
        // for(int i=0; i<k; i++)
        // {
        //     fprintf(out, "NEPROCCESAT: %s\n", lines[i]);
        //     fprintf(out, "PROCCESAT: x:%s ", x[i]);
        //     fprintf(out, " y:%s O: {", y[i]);
        //     for(int j=0; j<o_len[i]; j++) fprintf(out, "%s", o[i][j]);
        //     fprintf(out, "}\n");
        // }