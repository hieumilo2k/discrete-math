#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphInc.h"

char Vertex[MAX][6]; // danh sach dinh
int Matrix[MAX][MAX] = {};
int V;

void vedothi(char *filename, int V, int graph_matrix[MAX][MAX])
{
    FILE *fptr = fopen(filename, "w");
    fprintf(fptr, "graph{\n");

    for (int i = 0; i < V; i++)
    {
        for (int j = i + 1; j < V; j++)
        {
            if (graph_matrix[i][j] == 1)
            {
                fprintf(fptr, "%d -- %d;\n", i, j);
            }
        }
    }
    fprintf(fptr, "}");
    fclose(fptr);
}
void read_file(Graph g)
{
    char tmp[10] = "";
    IS is = new_inputstruct("sgb-words.txt");
    if(is == NULL){
        printf("Cannot open file !\n");
        exit(1);
    }
    int i = 0;
    while (get_line(is) >= 0)
    {
        if (is->NF >= 1)
        {
            strcpy(tmp, strdup(is->text1));
            tmp[strlen(is->text1) - 1] = '\0';
            strcpy(Vertex[i], tmp);
            addVertex(g, i, Vertex[i]);
            i++;
        }
    }
    V = i;
    jettison_inputstruct(is);
}

void init_edges(Graph g)
{
    int i, j, t, ktra = 0;
    for (i = 0; i < V; i++)
    {
        for (j = i + 1; j < V; j++)
        {
            for (t = 0; t < 5; t++)
            {
                if (Vertex[j][t] != Vertex[i][t])
                {
                    ktra++;
                }
            }
            if (ktra == 1)
            {
                Matrix[i][j] = 1;
                Matrix[j][i] = 1;
                addEdge(g, i, j, 1);
                addEdge(g, j, i, 1);
            }
            ktra = 0;
        }
    }
}

void TimDuong(Graph g, int s, int t, int *path, int *length)
{

    int visited[5757] = {};
    double distance[MAX], min, w, total;
    int previous[MAX], tmp[MAX];
    int n, output[MAX], i, u, v, start;
    Dllist ptr, queue, node;

    for (i = 0; i < MAX; i++)
        distance[i] = INFINITIVE_VALUE;
    distance[s] = 0;
    previous[s] = s;

    queue = new_dllist();
    dll_append(queue, new_jval_i(s));
    while (!dll_empty(queue))
    {

        min = INFINITIVE_VALUE;
        ptr = dll_first(queue);

        u = jval_i(ptr->val);
        dll_delete_node(ptr);

        if (!visited[u])
        {
            visited[u] = 1;
            if (u == t)
                break; // stop at t

            n = getAdjacentVertices(g, u, output);
            for (i = 0; i < n; i++)
            {
                v = output[i];
                w = getEdgeValue(g, u, v);
                if (distance[v] > distance[u] + w && v != previous[u])
                {
                    distance[v] = distance[u] + w;
                    previous[v] = u;
                }

                if(visited[v] == 0)
                    dll_append(queue, new_jval_i(v));

            }
        }
    }
    total = distance[t];

    if (total != INFINITIVE_VALUE)
    {
        tmp[0] = t;
        n = 1;
        while (t != s)
        {
            t = previous[t];
            tmp[n++] = t;
        }
        for (i = n - 1; i >= 0; i--)
            path[n - i - 1] = tmp[i];
        *length = n;
    }
}

int main()
{
    Graph g = createGraph();
    int i, j;
    int output[5757];
    int path[5757];
    int count = 0;
    int length;
    char u[10], v[10];
    read_file(g);
    init_edges(g);
    vedothi("graph1.dot", V, Matrix);
    printf("So thanh phan lien thong cua do thi G la : %d\n", tplienthong(g));
    do{
        printf("Nhap tu bat dau u: ");scanf("%s",u);
        for(i=0; i < V; i++){
    	    if(strcmp(Vertex[i], u) == 0){
                count++;
    		    break;
    	    }
        }
        if(count != 1)
            printf("Khong tim thay tu!\n");
    }while(count != 1);

    do{
        printf("Nhap tu ket thuc v: ");scanf("%s",v);
        for(j = 0; j < V; j++){
        	if(strcmp(Vertex[j], v)==0){
            count++;
    		break;
    	    }
        }
        if(count != 2)
            printf("Khong tim thay tu!\n");
    }while(count != 2);

    TimDuong(g, i, j, path, &length);

    for (int t = 0; t < length; t++)
        printf("%s\n", getVertex(g, path[t]));

    dropGraph(g); 
    return 0;
}