#ifndef __JRB_GRAPH__
#define __JRB_GRAPH__

#include "libfdr\jrb.h"
#include "libfdr\dllist.h"
#include "libfdr\fields.h"
#define INFINITIVE_VALUE  10000000
#define MAX 10000


typedef struct {
   JRB edges;
   JRB vertices;
} Graph;

Graph createGraph();
void addVertex(Graph graph, int id, char* name);
char *getVertex(Graph graph, int id);
int getVertex_id(Graph graph,char* name);
void addEdge(Graph graph, int v1, int v2, double weight);
int hasEdge(Graph graph, int v1, int v2);
int hasVertex(Graph graph, int id);
double getEdgeValue(Graph graph, int v1, int v2);
int getAdjacentVertices(Graph graph, int v, int *output);
void list_graph(Graph graph,int* output); // List adjacent of all vertices of graph
int indegree(Graph graph, int v, int* output);
int outdegree(Graph graph, int v, int* output);
int DAG(Graph graph);
int getGraphSize(Graph graph);
int getMaxId(Graph graph);
int getMinId(Graph graph);
void dropGraph(Graph graph);
void BFS(Graph graph, int start, int stop, void (*func)(int u));
void DFS(Graph graph, int start, int stop, void (*func)(int u));
double shortestPath(Graph graph, int s, int t, int* path, int*length);
int tplienthong(Graph g);
void printVertex(int v);
void printShortestPath(Graph g, int start, int stop);
int topoSort(Graph g, int* output);
int relax(Graph g, int u, int v, JRB parentList);
void changeEdgeWeight(Graph graph, int v1, int v2, double newWeight);
void deleteEdge(Graph graph, int v1, int v2);
void deleteVertex(Graph graph, int id);
#endif