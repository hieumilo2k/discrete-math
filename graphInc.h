#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include ".\libfdr\jrb.h"
#include ".\libfdr\dllist.h"
#include ".\libfdr\fields.h"
#define INFINITIVE_VALUE  10000000
#define MAX 10000

char *mau[]={"","green","red","blue","yellow","brown","purple", "ghostwhite", "silver","lawngreen","peachpuff","lightskyblue"};

int graph_matrix[MAX][MAX];
int vertex, edge;
int vertex_color[MAX];


typedef struct {
   JRB edges;
   JRB vertices;
} Graph;

Graph createGraph();
void addVertex(Graph graph, int id, char* name);
char *getVertex(Graph graph, int id);
void addEdge(Graph graph, int v1, int v2, double weight);
int hasEdge(Graph graph, int v1, int v2);
double getEdgeValue(Graph graph, int v1, int v2);
int getAdjacentVertices(Graph graph, int v, int *output);
int indegree(Graph graph, int v, int* output);
int outdegree(Graph graph, int v, int* output);
int DAG(Graph graph);
void dropGraph(Graph graph);
void BFS(Graph graph, int start, int stop);
void DFS(Graph graph, int start, int stop);
void TimDuong(Graph g, int s, int t, int *path, int *length);
int topoSort(Graph g, int* output);
double shortestPath(Graph graph, int s, int t, int* path, int*length);
int tplienthong(Graph g);
void printVertex(int v);
void clear_buffer ();
void PrimMST(Graph graph, int numV, Dllist list);
void tomau();