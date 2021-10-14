#include <stdio.h>
#include <stdlib.h>
#define MAX 50

char *mau[]={"","green","red","blue","yellow","brown","purple", "ghostwhite", "silver","lawngreen","peachpuff","lightskyblue"};

int graph_matrix[MAX][MAX];
int vertex, edge;
int vertex_color[MAX];

void docfile()
{
    int i, j;
    int p, q;
    FILE *fp;
    fp = fopen("dothi.txt", "r");
    if (fp == NULL)
    {
        printf("Khong tim thay file !");
        exit(1);
    }
    else
    {
        fscanf(fp, "%d", &vertex);
        printf("So dinh la: %d\n", vertex);
        fscanf(fp, "%d", &edge);
        printf("So canh la: %d\n", edge);
        for (i = 1; i <= vertex; i++)
            for (j = 1; j <= vertex; j++)
                graph_matrix[i][j] = 0;

        while (!feof(fp))
        {
            fscanf(fp, "%d", &p);
            fscanf(fp, "%d", &q);
            graph_matrix[p][q] = 1;
            graph_matrix[q][p] = 1;
        }
    }
}
void matranke()
{
    int i, j;
    for (i = 1; i <= vertex; i++)
    {
        for (j = 1; j <= vertex; j++)
            printf("%d ", graph_matrix[i][j]);
        printf("\n");
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

void vedothi(char *filename, int vertex, int graph_matrix[MAX][MAX], int *vertex_color)
{
    FILE *fptr = fopen(filename, "w");
    fprintf(fptr, "graph{\n");
    for (int i = 1; i <= vertex; i++)
    {
        fprintf(fptr, "%d [fillcolor=%s, style=filled];\n", i, mau[vertex_color[i]]);
    }

    for (int i = 1; i <= vertex; i++)
    {
        for (int j = i + 1; j <= vertex; j++)
        {
            if (graph_matrix[i][j] == 1)
            {
                fprintf(fptr, "%d -- %d;\n", i, j);
            }
        }
    }
    fprintf(fptr, "}");
    fclose(fptr);
}

void main()
{
    docfile();
    matranke();
    tomau();
    vedothi("graph.dot", vertex, graph_matrix, vertex_color);
}