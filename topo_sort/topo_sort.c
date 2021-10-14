#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphInc.h"

void vedothi(char *filename, Graph g, int vertex, int root[], int leaf[])
{
    FILE *fptr = fopen(filename, "w");
    fprintf(fptr, "digraph{\n");

    for (int i = 0; i < vertex - 1; i++)
    {
        fprintf(fptr, "%s -> %s ;\n", getVertex(g, root[i]), getVertex(g, leaf[i]));
    }
    fprintf(fptr, "}");
    fclose(fptr);
}

int main(){
	Graph g = createGraph();
    addVertex(g, 0, "Class102"); addVertex(g, 1, "Class140");
    addVertex(g, 2, "Class160"); addVertex(g, 3, "Class302");
    addVertex(g, 4, "Class311"); addVertex(g, 5, "MATH300");
    int vertex = 6;
    int root[vertex], leaf[vertex];

    addEdge(g, 0, 1, 1); root[0] = 0; leaf[0] = 1;
    addEdge(g, 0, 2, 1); root[1] = 0; leaf[1] = 2;
    addEdge(g, 1, 3, 1); root[2] = 1; leaf[2] = 3;
    addEdge(g, 5, 4, 1); root[3] = 5; leaf[3] = 4;
    addEdge(g, 3, 4, 1); root[4] = 3; leaf[4] = 4;
    vedothi("graph.dot", g, vertex, root, leaf);
    if (!DAG(g)) { 
       printf("Can not make topological sort\n"); 
       return 1; 
    }
    int output[20];
    int n = topoSort(g, output);
    printf("The topological order:\n");
    for (int i = 0; i < n; i++)
		 printf("%s\n", getVertex(g, output[i]));
}