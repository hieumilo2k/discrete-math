#include "graphInc.h"

Graph createGraph()
{
  Graph g;
  g.edges = make_jrb();
  g.vertices = make_jrb();
  return g;
}

void addVertex(Graph g, int id, char *name)
{
  JRB node = jrb_find_int(g.vertices, id);
  if (node == NULL) // only add new vertex
    jrb_insert_int(g.vertices, id, new_jval_s(strdup(name)));
}

char *getVertex(Graph g, int id)
{
  JRB node = jrb_find_int(g.vertices, id);
  if (node == NULL)
    return NULL;
  else
    return jval_s(node->val);
}

void addEdge(Graph graph, int v1, int v2, int weight)
{
  JRB node, tree;
  if (getEdgeValue(graph, v1, v2) == INFINITIVE_VALUE)
  {
    node = jrb_find_int(graph.edges, v1);
    if (node == NULL)
    {
      tree = make_jrb();
      jrb_insert_int(graph.edges, v1, new_jval_v(tree));
    }
    else
    {
      tree = (JRB)jval_v(node->val);
    }
    jrb_insert_int(tree, v2, new_jval_i(weight));
  }
}

int getEdgeValue(Graph graph, int v1, int v2)
{
  JRB node, tree;
  node = jrb_find_int(graph.edges, v1);
  if (node == NULL)
    return INFINITIVE_VALUE;
  tree = (JRB)jval_v(node->val);
  node = jrb_find_int(tree, v2);
  if (node == NULL)
    return INFINITIVE_VALUE;
  else
    return jval_i(node->val);
}

int indegree(Graph graph, int v, int *output)
{
  JRB tree, node;
  int total = 0;
  jrb_traverse(node, graph.edges)
  {
    tree = (JRB)jval_v(node->val);
    if (jrb_find_int(tree, v))
    {
      output[total] = jval_i(node->key);
      total++;
    }
  }
  return total;
}

int outdegree(Graph graph, int v, int *output)
{
  JRB tree, node;
  int total;
  node = jrb_find_int(graph.edges, v);
  if (node == NULL)
    return 0;
  tree = (JRB)jval_v(node->val);
  total = 0;
  jrb_traverse(node, tree)
  {
    output[total] = jval_i(node->key);
    total++;
  }
  return total;
}

int dijiktra(Graph g, int s, int t, int numV, int *path, int *length)
{
  int *isSelectMin = (int *)calloc(numV, sizeof(int));
  int *inPQ = (int *)calloc(numV, sizeof(int));
  int distance[numV], min_dist, w, total;
  int min, u;
  int previous[numV], tmp[numV];
  int n, output[numV], v;
  Dllist ptr, queue, node;

  for (int i = 0; i < numV; i++)
    distance[i] = INFINITIVE_VALUE;
  distance[s] = 0;
  previous[s] = s;

  queue = new_dllist();
  dll_append(queue, new_jval_i(s));

  while (!dll_empty(queue))
  {
    // get u from the priority queue
    min_dist = INFINITIVE_VALUE;
    dll_traverse(ptr, queue)
    {
      u = jval_i(ptr->val);
      if (min_dist > distance[u])
      {
        min_dist = distance[u];
        min = u;
        node = ptr;
      }
    }
    dll_delete_node(node);
    u = min;
    inPQ[u] = 0;

    if (u == t)
      break;

    isSelectMin[u] = 1;
    n = outdegree(g, u, output);
    for (int i = 0; i < n; i++)
    {
      v = output[i];
      if (!isSelectMin[v])
      {
        w = getEdgeValue(g, u, v);
        if (distance[v] > distance[u] + w)
        {
          distance[v] = distance[u] + w;
          previous[v] = u;
        }
        if (!inPQ[v])
        {
          dll_append(queue, new_jval_i(v));
          inPQ[v] = 1;
        }
      }
    }
  }

  free(isSelectMin);
  free(inPQ);
  free(queue);

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
    for (int i = n - 1; i >= 0; i--)
      path[n - i - 1] = tmp[i];
    *length = n;
  }

  return total;
}

void dropGraph(Graph graph)
{
  JRB node, tree;
  jrb_traverse(node, graph.edges)
  {
    tree = (JRB)jval_v(node->val);
    jrb_free_tree(tree);
  }

  jrb_free_tree(graph.edges);
  jrb_free_tree(graph.vertices);
}