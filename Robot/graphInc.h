#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libfdr/jrb.h"
#include "libfdr/dllist.h"

#define INFINITIVE_VALUE 10000000
#define MAX 10000
#define Queue Dllist
#define Stack Dllist

typedef struct
{
  int first;
  int second;
} Pairs;

typedef struct
{
  JRB edges;
  JRB vertices;
} Graph;

Graph createGraph();
void addVertex(Graph graph, int id, char *name);
char *getVertex(Graph graph, int id);
void addEdge(Graph graph, int v1, int v2, int weight);
int getEdgeValue(Graph graph, int v1, int v2);
int indegree(Graph graph, int v, int *output);
int outdegree(Graph graph, int v, int *output);
void dropGraph(Graph graph);
int dijiktra(Graph graph, int s, int t, int numV, int *path, int *length);