#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <string.h>
#include "graphInc.h"

char Vertex[MAX][6];
int **Matrix, **Matrix_t, **Matrix_c, V, Vc;
int *pre, *check, *post, clock, **LTM, *del, dem = 0, demdinh = 0;

void vedothi(char *filename, int V, int **graph_matrix)
{
    FILE *fptr = fopen(filename, "w");
    fprintf(fptr, "graph{\n");

    for (int i = 0; i < V; i++)
    {
        for (int j = i + 1; j < V; j++)
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

// tao dinh
void read_file(Graph g)
{
    char tmp[10] = "";
    IS is = new_inputstruct("sgb-words.txt");
    if (is == NULL)
    {
        printf("Cannot open file !\n");
        exit(1);
    }
    int i = 0;
    while (get_line(is) >= 0)
    {
        if (is->NF >= 1)
        {
            strcpy(tmp, strdup(is->text1));
            tmp[strlen(is->text1) - 1] = '\0';
            strcpy(Vertex[i], tmp);
            addVertex(g, i, Vertex[i]);
            i++;
        }
    }
    V = i;
    jettison_inputstruct(is);
}
//tao canh D
void taocanhD(Graph g)
{
    int i, j, t, giong = 0, z, G[4], k, ktra = 0;
    Matrix = (int **)malloc(V * sizeof(int *));
    for (i = 0; i < V; i++)
    {
        Matrix[i] = (int *)malloc(V * sizeof(int));
    }
    for (i = 0; i < V; i++)
    {
        for (j = 0; j < V; j++)
        {
            Matrix[i][j] = 0;
        }
    }
    for (k = 0; k < 4; k++)
    { // luu cac vi tri da duoc quet cua xau 2
        G[k] = -1;
    }
    for (i = 0; i < V; i++)
    {
        for (j = 0; j < V; j++)
        {
            if (i != j)
            {
                for (t = 1; t < 5; t++)
                { // ky ty thu t cua xau 1
                    for (z = 0; z < 5; z++)
                    { // ky tu thu z cua xau 2
                        for (k = 0; k < 4; k++)
                        { // kiem tra vi tri z da duoc quet chua
                            if (z == G[k])
                            {
                                ktra = 1;
                                break;
                            }
                            else
                            {
                                ktra = 0;
                            }
                        }
                        if ((ktra == 0) && (Vertex[j][z] == Vertex[i][t]))
                        {                 // neu z chua duoc quet
                            G[giong] = z; // nhet z vao danh sach da quet
                            giong++;      // tang so ki tu giong
                            break;        // quet luon den z tiep theo
                        }
                    }
                }
                if (giong == 4)
                {
                    Matrix[i][j] = 1;
                    addEdge(g, i, j, 1);
                }
                // reset ve gia tri khoi tao
                giong = 0;
                for (k = 0; k < 4; k++)
                {
                    G[k] = -1;
                }
            }
        }
    }
}
// B.a
void initMatrix_t()
{ // Dt la do thi nguoc voi do thi D
    int i, j;
    Matrix_t = (int **)malloc(V * sizeof(int *));
    for (i = 0; i < V; i++)
    {
        Matrix_t[i] = (int *)malloc(V * sizeof(int));
    }
    for (i = 0; i < V; i++)
    {
        for (j = 0; j < V; j++)
        {
            if (Matrix[i][j] == 1)
                Matrix_t[j][i] = 1;
        }
    }
}

// thuat toan DFS
void initPrePost()
{
    pre = (int *)malloc(V * sizeof(int));
    post = (int *)malloc(V * sizeof(int));
}

void pr(int u)
{
    clock += 1;
    pre[u] = clock;
}

void pst(int u)
{
    clock += 1;
    post[u] = clock;
}

void explore(int u, int **G)
{
    int i;
    check[u] = 1;
    LTM[dem][demdinh] = u; // dua goc u vao thanh phan lien thong manh lmt[dem]
    demdinh++;
    pr(u);
    for (i = 0; i < V; i++)
    {
        if (G[u][i] == 1 && check[i] == 0)
        {
            explore(i, G);
        }
    }
    pst(u);
}

int maxPost()
{ // tim dinh co so post lon nhat
    int max = 0, i, chiso;
    for (i = 0; i < V; i++)
    {
        if (max < post[i] && post[i] >= 0)
        {
            max = post[i];
            chiso = i;
        }
    }
    post[chiso] = -1;
    return chiso;
}

int *sortPost()
{ // sap xep cac dinh de quet DFS theo thu tu giam dan so post
    int i, *sort;
    sort = (int *)malloc(sizeof(int) * V);
    for (i = 0; i < V; i++)
    {
        sort[i] = maxPost();
    }
    return sort;
}

void DFS(int **G)
{
    int i;
    clock = 0;
    check = (int *)malloc(V * sizeof(int));
    for (i = 0; i < V; i++)
        check[i] = 0;
    for (i = 0; i < V; i++)
    {
        if (check[i] == 0)
            explore(i, G);
    }
}

void initMatrix_c()
{ // khoi tao do thi giong do thi D de lam viec khong anh huong toi do thi D goc
    int i, j;
    Matrix_c = (int **)malloc(V * sizeof(int *));
    for (i = 0; i < V; i++)
    {
        Matrix_c[i] = (int *)malloc(V * sizeof(int));
    }
    for (i = 0; i < V; i++)
    {
        for (j = 0; j < V; j++)
            Matrix_c[i][j] = Matrix[i][j];
    }
}

void initListLTM()
{
    int i;
    LTM = (int **)malloc(V * sizeof(int *));
    for (i = 0; i < V; i++)
    {
        LTM[i] = (int *)malloc(V * sizeof(int));
    }
    del = (int *)malloc(V * sizeof(int));
    for (i = 0; i < V; i++)
    {
        del[i] = 0;
    }
    dem = 0;
    demdinh = 0;
}

void delEdge(int u)
{ // xoa dinh
    int i;
    for (i = 0; i < V; i++)
    {
        if (Matrix_c[u][i] == 1)
            Matrix_c[u][i] = 0;
    }
}

void DemLTM()
{ // xoa thanh phan lien thong manh hut vua tim duoc
    int *sort, i, j;
    sort = (int *)malloc(V * sizeof(int));
    initMatrix_t();
    initPrePost();
    initListLTM();
    DFS(Matrix_t);
    sort = sortPost();
    for (i = 0; i < V; i++)
        check[i] = 0;
    initMatrix_c();
    initListLTM();
    clock = 0;
    for (i = 0; i < V; i++)
    {
        if (check[sort[i]] == 0)
        {
            explore(sort[i], Matrix_c);
            for (j = 0; j < V; j++)
            {
                if (check[j] == 1)
                    delEdge(j);
            }
            demdinh = 0;
            dem++;
        }
    }
}

// B.b
// thu tuc xu ly xau U dua vao trong phan B.b
int findU(char *u)
{
    int i;
    for (i = 0; i < V; i++)
    {
        if (strcmp(u, Vertex[i]) == 0)
            return i;
    }
}
int findLTM(int u)
{
    int i, j;
    for (i = 0; i < dem; i++)
    {
        for (j = 0; j < V; j++)
        {
            if (LTM[i][j] == u)
                return i;
        }
    }
}
void print(int i)
{
    int j = 0;
    while (LTM[i][j] != 0)
    {
        printf("%s ", Vertex[LTM[i][j]]);
        j++;
    }
}

void TimDuong(Graph g, int s, int t, int *path, int *length)
{

    int visited[5757] = {};
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

                if (visited[v] == 0)
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

int main()
{
    Graph g = createGraph();
    int i, j, *sort;
    char word[6];
    int output[5757];
    int path[5757];
    int count = 0;
    int length;
    char u[10], v[10];
    int choice;
    read_file(g);
    taocanhD(g);
    vedothi("graph2.dot", V, Matrix);
    do
    {
        printf("\n<<===============MENU==============>>\n");
        printf("||1.So TPLT manh                   ||\n");
        printf("||2.Cac tu cung TPLT manh          ||\n");
        printf("||3.Tim duong di ngan nhat         ||\n");
        printf("||4.Thoat                          ||\n");
        printf("<<=================================>>\n");
        printf("Your choice ? ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printf("Loading...\n");

            // B.a	dem so thanh phan lien thong manh
            DemLTM();
            printf("Complete !\n");
            printf("So thanh phan lien thong manh la: %d\n", dem);
            break;

        case 2:
            // B.b nhap u, ghi ra ca tu cung TPLTM
            printf("Nhap tu : ");
            scanf("%s", word);
            printf("Cac tu cung thanh phan lien thong manh voi %s la: \n", word);
            print(findLTM(findU(word)));
            break;

        case 3:
            printf("Nhap tu bat dau u: ");
            scanf("%s", u);
            for (i = 0; i < V; i++)
            {
                if (strcmp(Vertex[i], u) == 0)
                {
                    count++;
                    break;
                }
            }

            printf("Nhap tu ket thuc v: ");
            scanf("%s", v);
            for (j = 0; j < V; j++)
            {
                if (strcmp(Vertex[j], v) == 0)
                {
                    count++;
                    break;
                }
            }
            TimDuong(g, i, j, path, &length);

            for (int t = 0; t < length; t++)
                printf("%s\n", getVertex(g, path[t]));
            break;

        case 4:
            // free memories
            for (i = 0; i < V; i++)
            {
                free(Matrix_c[i]);
                free(Matrix_t[i]);
                free(LTM[i]);
                free(Matrix[i]);
            }
            free(Matrix_c);
            free(Matrix_t);
            free(check);
            free(pre);
            free(post);
            free(LTM);
            free(del);
            free(Matrix);
            dropGraph(g);
            exit(0);

        default:
            printf("De nghi nghiem tuc. Nhap tu 1 -> 4");
            break;
        }

    } while (1);
}
