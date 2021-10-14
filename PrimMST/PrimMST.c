#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libfdr\dllist.h"
#include "libfdr\fields.h"
#include "graphInc.h"

void vedothi(char *filename, int vertex, int root[], int leaf[], double weight[])
{
    FILE *fptr = fopen(filename, "w");
    fprintf(fptr, "graph{\n");

    for (int i = 0; i < vertex; i++)
    {
        fprintf(fptr, "%d -- %d [label=\"%g\"];\n", root[i], leaf[i], weight[i]);
    }
    fprintf(fptr, "}");
    fclose(fptr);
}

void PrimMST(Graph graph, int numV, Dllist list){
	int output[MAX];
	int visited[MAX] = {};
	int comp[MAX] = {};
	int count = 1;
	int min = INFINITIVE_VALUE;
	Dllist queue, ptr, node;
	queue = new_dllist();
	int n, vertex1 = 0;
	visited[0] = 1;
	comp[0] = 1;
	n = outdegree(graph, vertex1, output);
	for(int i = 0; i < n; i++){
		dll_append(queue, new_jval_iarray(vertex1, output[i]));
	}
	while (!dll_empty(queue))
	{
		min = INFINITIVE_VALUE;
		count = 0;
		dll_traverse(ptr, queue){
			if(getEdgeValue(graph, ptr->val.iarray[0], ptr->val.iarray[1]) < min){
				min = getEdgeValue(graph, ptr->val.iarray[0], ptr->val.iarray[1]);
				vertex1 = ptr->val.iarray[1];
				node = ptr;
			}
		}
		dll_append(list, new_jval_iarray(node->val.iarray[0], node->val.iarray[1]));
		comp[node->val.iarray[0]] = 1;
		comp[node->val.iarray[1]] = 1;
		dll_delete_node(node);
		for(int i = 0; i < numV; i++){
			if(comp[i])
				count++;
		}
		if(count == numV)
			return;
		visited[vertex1] = 1;
		n = outdegree(graph, vertex1, output);
		for(int i = 0; i < n; i++){
			if(!visited[output[i]])
				dll_append(queue, new_jval_iarray(vertex1, output[i]));
		}
	}
	
}

int main(){
	Graph g = createGraph();
    FILE * fp;
    if((fp = fopen("graph.txt", "r")) == NULL){
        printf("Can not open file !\n");
        exit(1);
    }
    int vertex;
    fscanf(fp, "%d%*c", &vertex);
    int root[vertex];
    int leaf[vertex];
	double weight[vertex];
    for(int i = 0; i < vertex; i++){
        fscanf(fp, "%d %d %lf%*c", &root[i], &leaf[i], &weight[i]);
		addEdge(g, root[i], leaf[i], weight[i]);
		addEdge(g, leaf[i], root[i], weight[i]);
    }
	vedothi("graph.dot", vertex, root, leaf, weight);

	int count = 1;
	int j = 0;
	Dllist list, ptr;
	list = new_dllist();
	PrimMST(g, vertex, list);
	printf("Edge	 Weight\n");
	dll_traverse(ptr, list){
		printf("%d - %d : %4g\n", ptr->val.iarray[0], ptr->val.iarray[1], getEdgeValue(g, ptr->val.iarray[0], ptr->val.iarray[1]));
		root[j] = ptr->val.iarray[0];
		leaf[j] = ptr->val.iarray[1];
		weight[j++] = getEdgeValue(g, ptr->val.iarray[0], ptr->val.iarray[1]);
		count = j;
	}
	vedothi("graphPrimMST.dot", count, root, leaf, weight);
	dropGraph(g);
	fclose(fp);
}