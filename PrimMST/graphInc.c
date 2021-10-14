#include "graphInc.h"

Graph createGraph()
{
   Graph g; 
   g.edges = make_jrb();  
   g.vertices = make_jrb();  
   return g;
}

void addVertex(Graph g, int id, char* name)
{
     JRB node = jrb_find_int(g.vertices, id);
     if (node==NULL) // only add new vertex 
         jrb_insert_int(g.vertices, id, new_jval_s(strdup(name)));            
}

char *getVertex(Graph g, int id)
{
     JRB node = jrb_find_int(g.vertices, id);
     if (node==NULL) 
        return NULL;
     else                
        return jval_s(node->val);
}     

int getAdjacentVertices(Graph graph, int v, int *output){
	JRB node = jrb_find_int(graph.edges, v);
	int total = 0;
	if(node != NULL){
		JRB tree = (JRB)jval_v(node->val);
		JRB ptr;
		jrb_traverse(ptr, tree){
			output[total++] = jval_i(ptr->key);
		}
	}
	return total;
}


void addEdge(Graph graph, int v1, int v2, double weight)
{
     JRB node, tree;
     if (getEdgeValue(graph, v1, v2)==INFINITIVE_VALUE)
     {
        node = jrb_find_int(graph.edges, v1);
        if (node==NULL) {
           tree = make_jrb();
           jrb_insert_int(graph.edges, v1, new_jval_v(tree));
        } else
        {
           tree = (JRB) jval_v(node->val);   
        }
        jrb_insert_int(tree, v2, new_jval_d(weight));
     }
}

int hasEdge(Graph graph, int v1, int v2)
{
   JRB node, tree;
   node = jrb_find_int(graph.edges, v1);
   if (node == NULL)
      return 0;
   tree = (JRB)jval_v(node->val);
   if (jrb_find_int(tree, v2) == NULL)
      return 0;
   else
      return 1;
}

double getEdgeValue(Graph graph, int v1, int v2)
{
    JRB node, tree;
    node = jrb_find_int(graph.edges, v1);
    if (node==NULL)
       return INFINITIVE_VALUE;
    tree = (JRB) jval_v(node->val);
    node = jrb_find_int(tree, v2);
    if (node==NULL)
       return INFINITIVE_VALUE;
    else
       return jval_d(node->val);       
}

int indegree (Graph graph, int v, int* output)
{
    JRB tree, node;
    int total = 0;   
    jrb_traverse(node, graph.edges)
    {
       tree = (JRB) jval_v(node->val);
       if (jrb_find_int(tree, v))
       {
          output[total] = jval_i(node->key);
          total++;
       }                
    }
    return total;   
}

int outdegree (Graph graph, int v, int* output)
{
    JRB tree, node;
    int total;
    node = jrb_find_int(graph.edges, v);
    if (node==NULL)
       return 0;
    tree = (JRB) jval_v(node->val);
    total = 0;   
    jrb_traverse(node, tree)
    {
       output[total] = jval_i(node->key);
       total++;                
    }
    return total;   
}

void printVertex(int v)
{
     printf("%4d", v);
}

double shortestPath(Graph g, int s, int t, int* path, int*length)
{
   double distance[1000], min, w, total;
   int previous[1000], tmp[1000];
   int n, output[100], i, u, v, start;
   Dllist ptr, queue, node;

   for (i=0; i<1000; i++)
       distance[i] = INFINITIVE_VALUE;
   distance[s] = 0;
   previous[s] = s;
       
   queue = new_dllist();
   dll_append(queue, new_jval_i(s));

   while ( !dll_empty(queue) )
   {
      // get u from the priority queue   
      min = INFINITIVE_VALUE;   
      dll_traverse(ptr, queue)
      {
          u = jval_i(ptr->val);              
          if (min > distance[u])
          {
             min = distance[u];
             node = ptr;
          }                 
      }
      dll_delete_node(node);
      
      if (u == t) break; // stop at t
      
      n = outdegree(g, u, output);
      for (i=0; i<n; i++)
      {
          v = output[i];
          w = getEdgeValue(g, u, v);
          if ( distance[v] > distance[u] + w )
          {    
              distance[v] = distance[u] + w;
              previous[v] = u;
          }     
          dll_append(queue, new_jval_i(v));
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
       for (i=n-1; i>=0; i--)
           path[n-i-1] = tmp[i];
       *length = n;                
   }
   return total;   
}

void dropGraph(Graph graph)
{
    JRB node, tree;
    jrb_traverse(node, graph.edges)
    {
        tree = (JRB) jval_v(node->val);
        jrb_free_tree(tree);
    }
    jrb_free_tree(graph.edges);
    jrb_free_tree(graph.vertices);
}


void BFS(Graph graph, int start, int stop)
{
   int visited[5757] = {};
   int n, output[5757], i, u, v;
   Dllist node, queue;
   
   queue = new_dllist();
   dll_append(queue, new_jval_i(start));

   while ( !dll_empty(queue) )
   {
      node = dll_first(queue);
      u = jval_i(node->val);
      dll_delete_node(node);
      if (!visited[u]) 
      {
         printf("%4s ", getVertex(graph, u));
          visited[u] = 1;
          if ( u == stop ) return;           
          n = getAdjacentVertices(graph, u, output);
          for (i=0; i<n; i++)
          {
              v = output[i];
              if (!visited[v])
                 dll_append(queue, new_jval_i(v));
          }
      }
   }                            
}

// void DFS(Graph graph, int start, int stop)
// {
//    int visited[5757] = {};
//    int n, output[5757], i, u, v;
//    Dllist node, stack;

//    stack = new_dllist();
//    dll_append(stack, new_jval_i(start));

//    while ( !dll_empty(stack) )
//    {
//       node = dll_last(stack);
//       u = jval_i(node->val);
//       dll_delete_node(node);
//       if (!visited[u]) 
//       {
//           visited[u] = 1;
//           if ( u == stop ) return;           
//           n = getAdjacentVertices(graph, u, output);
//           for (i=0; i<n; i++)
//           {
//               v = output[i];
//               if (!visited[v])
//                  dll_append(stack, new_jval_i(v));
//           }
//       }
//    }                            
// }

int tplienthong(Graph g)
{
  Dllist queue = new_dllist(), node;
  int visited[MAX], out[MAX];
  int i, u, v, n, total = 0;
  for(i=0 ;i<MAX ;i++ ) visited[i] = 0;
  JRB  tmp;
  jrb_traverse(tmp,g.vertices){
    u = tmp->key.i;
    if(!visited[u])
      {
        total++;
        dll_append(queue, new_jval_i(u));
        while(!dll_empty(queue))
          {
            node = dll_last(queue);
            v = jval_i(node->val);
            dll_delete_node(node);
            if(!visited[v])
              {
                visited[v] = 1;
                n = getAdjacentVertices(g, v, out);
                for( i=0;i < n ; i++ )
                  {
                    v = out[i];
                    if(!visited[v]) dll_append(queue, new_jval_i(v));
                  }
              }
          }
      }
  }
  return total;
}

void clear_buffer ()
{
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF);
}