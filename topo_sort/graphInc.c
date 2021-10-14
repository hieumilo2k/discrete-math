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


int getVertex_id(Graph graph,char* name){
	JRB node;
	int check = 0;
	jrb_traverse(node,graph.vertices){
		char* getName = strdup(jval_s(node->val)); 
		if(strcmp(name,getName) == 0) {
			check = 1;
			break;
		}
	}
	if(check == 1)
		return jval_i(node->key);
	else return -1;
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

void list_graph(Graph graph,int* output){
  JRB node,name;
  jrb_traverse(node,graph.vertices){
    int v = jval_i(node->key);
    name = jrb_find_int(graph.vertices,v);
    printf("Vertex %d : ",jval_i(name->val));
    int n = outdegree(graph,v,output);
    if(n != 0){
      for(int i = 0;i<n;i++){
        name = jrb_find_int(graph.vertices,output[i]);
        printf("--> %d ",jval_i(name->val));
      }
    }
    printf("\n");
  }
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

int hasVertex(Graph graph, int id){
	return (jrb_find_int(graph.vertices, id) != NULL);
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

int getMaxId(Graph graph) {
	if (graph.edges == NULL || graph.vertices == NULL)
		return 0;
	int max_id = 0;
	JRB tmp;
	jrb_traverse(tmp, graph.vertices) {
		int key = jval_i(tmp->key);
		if (key > max_id)
			max_id = key;
	}
	return max_id;
}

int getMinId(Graph graph) {
	if (graph.edges == NULL || graph.vertices == NULL)
		return 0;
	int min_id = 10000000;
	JRB tmp;
	jrb_traverse(tmp, graph.vertices) {
		int key = jval_i(tmp->key);
		if (key < min_id)
			min_id = key;
	}
	return min_id;
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

int getGraphSize(Graph graph){
	int graph_size = 0;
	JRB node;
	jrb_traverse(node, graph.vertices){
		graph_size++;
	}
	return graph_size;
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

void doNothing(int u){}


void printVertex(int v)
{
     printf("%4d", v);
}

void printShortestPath(Graph g, int start, int stop){
	int path[200];
    int length;
	int total = shortestPath(g, start, stop, path, &length);
	for(int i = 0; i < total; i++)
		printf("%-5d", path[i]);
}

void BFS(Graph graph, int start, int stop, void (*func)(int))
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
        func(u);
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

void DFS(Graph graph, int start, int stop, void (*func)(int))
{
   int visited[5757] = {};
   int n, output[5757], i, u, v;
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
        func(u);
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

int topoSort(Graph g, int* output){
	int total = 0;
	Dllist queue = new_dllist();

	int inNodes[20];
	int outNodes[20];

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

int relax(Graph g, int u, int v, JRB parentList){
	int l = getEdgeValue(g, u, v);
	JRB fromNode = jrb_find_int(g.vertices, u);
	JRB toNode = jrb_find_int(g.vertices, v);
	int d = jval_i(fromNode->val) + l;
	if(d < jval_i(toNode->val)){
		toNode->val = new_jval_i(d);
		JRB parent_v = jrb_find_int(parentList, v);
		parent_v->val = new_jval_i(u);
		return 1; //find shorter path
	}
	return 0; //can not update shorter path

}

void changeEdgeWeight(Graph graph, int v1, int v2, double newWeight){
	JRB tmp = jrb_find_int(graph.edges, v1);
	JRB tmp1 = NULL;
		if(tmp == NULL) return;

	tmp = (JRB) tmp->val.v;

	if((tmp1 = jrb_find_int(tmp, v2)) == NULL)
		return;

	tmp1->val.d = newWeight;
}

void deleteEdge(Graph graph, int v1, int v2){ 	//return 1 if succeed, 0 otherwise
	if(!hasEdge(graph, v1, v2)) return;

	JRB tmp = jrb_find_int(graph.edges, v1);
	if(tmp == NULL) return;

	tmp = (JRB) tmp->val.v;

	if((tmp = jrb_find_int(tmp, v2)) == NULL)
		return;
	else jrb_delete_node(tmp);

} 

void deleteVertex(Graph graph, int id){

	JRB vertex = NULL;
	JRB vertex_tree = NULL;
	JRB tmp = NULL;

	if((tmp = jrb_find_int(graph.vertices, id)) == NULL) return;
	jrb_delete_node(tmp);

	jrb_traverse(vertex, graph.edges){
		if(vertex->key.i != id){
			vertex_tree = (JRB) vertex->val.v;

			jrb_traverse(tmp, vertex_tree){
				if(tmp->key.i == id)
					jrb_delete_node(tmp);
			}
		}
		else jrb_delete_node(vertex);
	}
}