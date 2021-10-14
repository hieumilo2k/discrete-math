#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libfdr/jrb.h"
#include "libfdr/dllist.h"
#include "libfdr/fields.h"
#include "graphInc.h"
#include "stack_dllist.h"
#include "queue_dllist.h"

int a, b, c, d, r, m, n;

int x, y;

int Matrix[MAX][MAX] = {};
int weight[MAX][MAX];

void vedothi(char *filename, int V, int graph_matrix[MAX][MAX], int weight[MAX][MAX])
{
    FILE *fptr = fopen(filename, "w");
    fprintf(fptr, "graph{\n");

    for (int i = 0; i < V; i++)
    {
        for (int j = i + 1; j < V; j++)
        {
            if (graph_matrix[i][j] == 1)
            {
                fprintf(fptr, "%d -- %d [label=\"%d\"];\n", i, j, weight[i][j]);
            }
        }
    }
    fprintf(fptr, "}");
    fclose(fptr);
}

void readFile(Graph g, const char *filename)
{
  int v1, v2, w;
  IS is;
  int count = 0;
  is = new_inputstruct(filename);
  if (get_line(is))
  {
    n = atoi(is->fields[0]);
    m = atoi(is->fields[1]);
  }
  for (int i = 0; i < m; i++)
  {
    if (get_line(is))
    {
      v1 = atoi(is->fields[0]);
      v2 = atoi(is->fields[1]);
      w = atoi(is->fields[2]);
      Matrix[v1][v2] = 1;
      weight[v1][v2] = w;
      addEdge(g, v1, v2, w);
      addEdge(g, v2, v1, w);
    }
  }

  while (get_line(is) >= 0)
  {
    if (count == 0)
    {
      a = atoi(is->fields[0]);
      b = atoi(is->fields[1]);
    }
    else if (count == 1)
    {
      c = atoi(is->fields[0]);
      d = atoi(is->fields[1]);
    }
    else
    {
      r = atoi(is->fields[0]);
    }
    count++;
  }
  jettison_inputstruct(is);
}

int main()
{
  char namefile[20];
  int nFind, v1, v2, w;
  Graph g = createGraph();
  printf("Nhap ten file: ");
  scanf("%s%*c", namefile);
  readFile(g, namefile);
  vedothi("graph.dot", n, Matrix, weight);
  int Path[n], length = 0;
  int dist[n][n];
  Pairs pairs[n][n];
  int output[n];

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      dist[i][j] = dijiktra(g, i, j, n, Path, &length);
    }
  }

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      pairs[i][j].first = pairs[i][j].second = 0;
    }
  }

  //Setup
  Queue q = queue_create();
  enQueue(q, new_jval_iarray(a, b));
  pairs[a][b].first = pairs[a][b].second = -1;

  while (!dll_empty(q))
  {
    int u = queue_front(q).iarray[0];
    int v = queue_front(q).iarray[1];
    deQueue(q);

    nFind = outdegree(g, u, output);
    for (int i = 0; i < nFind; i++)
    {
      int uu = output[i];
      if ((pairs[uu][v].first == 0) && (pairs[uu][v].second == 0) && (dist[uu][v] > r))
      {
        pairs[uu][v].first = u;
        pairs[uu][v].second = v;
        enQueue(q, new_jval_iarray(uu, v));
      }
    }

    nFind = outdegree(g, v, output);
    for (int i = 0; i < nFind; i++)
    {
      int vv = output[i];
      if ((pairs[u][vv].first == 0) && (pairs[u][vv].second == 0) && (dist[u][vv] > r))
      {
        pairs[u][vv].first = u;
        pairs[u][vv].second = v;
        enQueue(q, new_jval_iarray(u, vv));
      }
    }

    if (pairs[c][d].first != 0 && pairs[c][d].second != 0)
      break;
  }

  if (pairs[c][d].first == 0 && pairs[c][d].second == 0)
  {
    printf("Khong the\n");
  }
  else
  {
    // Print
    Stack s = stack_create();

    while (pairs[c][d].first != -1 && pairs[c][d].second != -1)
    {
      stack_push(s, new_jval_iarray(c, d));
      int x = pairs[c][d].first;
      int y = pairs[c][d].second;
      c = x;
      d = y;
    }

    printf("Lich di chuyen\t\tKhoang cach\n");
    printf("%d %d\t\t%19d\n", a, b, dist[a][b]);

    while (!stack_empty(s))
    {
      int first = stack_top(s).iarray[0];
      int second = stack_top(s).iarray[1];

      printf("%d %d\t\t%19d\n", first, second, dist[first][second]);
      stack_pop(s);
    }
  }

  printf("\n");

  dropGraph(g);
}