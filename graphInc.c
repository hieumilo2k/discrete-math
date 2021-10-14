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

int DAG(Graph graph)
{
   int visited[1000];
   int n, output[100], i, u, v, start;
   Dllist node, stack;
   JRB vertex;
   
   jrb_traverse(vertex, graph.vertices)
   {
       memset(visited, 0, sizeof(visited));

       start = jval_i(vertex->key);              
       stack = new_dllist();
       dll_append(stack, new_jval_i(start));
    
       while ( !dll_empty(stack) )
       {
          node = dll_last(stack);
          u = jval_i(node->val);
          dll_delete_node(node);
          if (!visited[u])
          {
              visited[u] = 1;
              n = outdegree(graph, u, output);
              for (i=0; i<n; i++)
              {
                  v = output[i];
                  if ( v == start ) // cycle detected 
                     return 0;
                  if (!visited[v])    
                     dll_append(stack, new_jval_i(v));
              }
          }           
       }
   }
   return 1; // no cycle    
}

void printVertex(int v)
{
     printf("%4d", v);
}

double shortestPath(Graph g, int s, int t, int* path, int*length)
{
   double distance[MAX], min, w, total;
   int previous[MAX], tmp[MAX];
   int n, output[MAX], i, u, v, start;
   Dllist ptr, queue, node;

   for (i=0; i<MAX; i++)
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
   int visited[MAX] = {};
   int n, output[MAX], i, u, v;
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

void DFS(Graph graph, int start, int stop)
{
   int visited[MAX] = {};
   int n, output[MAX], i, u, v;
   Dllist node, stack;

   stack = new_dllist();
   dll_append(stack, new_jval_i(start));

   while ( !dll_empty(stack) )
   {
      node = dll_last(stack);
      u = jval_i(node->val);
      dll_delete_node(node);
      if (!visited[u]) 
      {
          visited[u] = 1;
          if ( u == stop ) return;           
          n = getAdjacentVertices(graph, u, output);
          for (i=0; i<n; i++)
          {
              v = output[i];
              if (!visited[v])
                 dll_append(stack, new_jval_i(v));
          }
      }
   }                            
}

void TimDuong(Graph g, int s, int t, int *path, int *length)
{

    int visited[MAX] = {};
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

int topoSort(Graph g, int* output){
	int total = 0;
	Dllist queue = new_dllist();

	int inNodes[MAX];
	int outNodes[MAX];

	//make inDegree Table
	JRB inDegreeTable = make_jrb();
	JRB node;
	jrb_traverse(node, g.edges){
		int n = indegree(g, jval_i(node->key), inNodes);
		jrb_insert_int(inDegreeTable, jval_i(node->key), new_jval_i(n));
		if(n == 0)
			dll_append(queue, node->key);

		JRB tree = (JRB)jval_v(node->val);
		JRB ptr;
		jrb_traverse(ptr, tree){
			if(!jrb_find_int(inDegreeTable, jval_i(ptr->key))){
				n = indegree(g, jval_i(ptr->key), inNodes);
				jrb_insert_int(inDegreeTable, jval_i(ptr->key), new_jval_i(n));
				if(n == 0)
					dll_append(queue, ptr->key);
			}
		}
	}

	while(!dll_empty(queue)){
		int u = jval_i(dll_first(queue)->val);
		dll_delete_node(dll_first(queue));
		output[total++] = u;

		int outNodeNumber = outdegree(g, u, outNodes);
		for(int i = 0; i < outNodeNumber; i++){
			JRB out = jrb_find_int(inDegreeTable, outNodes[i]);
			out->val = new_jval_i(jval_i(out->val) - 1);
			if(jval_i(out->val) == 0)
				dll_append(queue, out->key);
		}
	}
	return total;
}

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

//kiem tra mau trung
int kiemtramau(int a[], int k, int color)
{
    int i;
    for (i = 1; i <= k; i++)
        if (color == a[i])
            return 1;
    return 0;
}
// chon mau thich hop de to
int chonmau(int g[], int l)
{
    int color = 1;
    int i;
    do
    {
        if (kiemtramau(g, l, color) == 0)
        {
            return color;
            break;
        }
        else
            color++;
    } while (1);
}
// to mau cho cac dinh do thi
void tomau()
{
    int i, j, l, count;
    int color, t;
    int dinhtruoc[MAX], mauphu[MAX];
    i = l = color = t = 1;
    j = 2;
    vertex_color[1] = color;
    dinhtruoc[1] = 1; 

    do
    {
        for (count = 1; count <= l; count++)
            if (graph_matrix[j][dinhtruoc[count]] == 1)
                mauphu[++t] = vertex_color[dinhtruoc[count]];

        vertex_color[j] = chonmau(mauphu, t);
        l = j;
        dinhtruoc[l] = j; 
        j++;
        t = 0;
    } while (j <= vertex);
    for (i = 1; i <= vertex; i++)
        printf("mau dinh % 3d: % 5d\n", i, vertex_color[i]);
}
