#include<stdio.h>
#include <stdlib.h>
#include "GenDLL.h"

void vedothi(char *filename, int vertex, int root[], int leaf[])
{
    FILE *fptr = fopen(filename, "w");
    fprintf(fptr, "graph{\n");

    for (int i = 0; i < vertex; i++)
    {
        fprintf(fptr, "%d -- %d;\n", root[i], leaf[i]);
    }
    fprintf(fptr, "}");
    fclose(fptr);
}


int main() {
    FILE * fp;
    if((fp = fopen("prufer.txt", "r")) == NULL){
        printf("Can not open file !\n");
        exit(1);
    }
    int vertex;
    fscanf(fp, "%d%*c", &vertex);
    int root[vertex];
    int leaf[vertex];
    for(int i = 0; i < vertex; i++){
        fscanf(fp, "%d %d%*c", &root[i], &leaf[i]);
    }
    vedothi("graph.dot", vertex, root, leaf);
    listnode_dll rootPruferCode;
    listnode_dll tailPruferCode;
    listnode_dll currentPruferCode = NULL;
    initialize(&rootPruferCode, &tailPruferCode);

    int *element = (int*)calloc(vertex, sizeof(int));
    
    for (int i = 0; i < vertex; i++) { 
        element[root[i]]+= 1;
        element[leaf[i]]+= 1;
    }
  
    int min = 1;  
    while (element[min] > 1) 
        min++;
    for (int i = 0; i < vertex - 1 ; i++) {
        for (int j = 0; j < vertex; j++) {
            if (root[j] == min) {
                element[min] += 1;
                root[j] = 0;
                insertAtTail(leaf[j], &rootPruferCode, &tailPruferCode);
                element[leaf[j]] -= 1;
                leaf[j] = 0;
                break;
           
            }
            if (leaf[j] == min) {
                element[min] += 1;
                leaf[j] = 0;
                insertAtTail(root[j], &rootPruferCode, &tailPruferCode);
                element[root[j]] -= 1;
                root[j] = 0;
                break;
            }
        }
        min = 1;  
        while (element[min] > 1) 
            min++;
    }
    printf("Ma Prufer code cua cay nay la: ");
	traverseListFromRoot(rootPruferCode);
    clean_up(&rootPruferCode);
    fclose(fp);
    return 0;
}