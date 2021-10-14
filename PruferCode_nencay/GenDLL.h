typedef int ElementType_dll;

typedef struct node_t{
    ElementType_dll element;
    struct node_t *next;
    struct node_t *prev;
}node;

typedef node* listnode_dll;

void initialize(listnode_dll *root, listnode_dll *tail){
    *root = NULL;
    *tail = NULL;
}

node* makeNewNode(ElementType_dll add){
    node* new = (node*)malloc(sizeof(node));
    if(new == NULL){
        printf("Khong du bo nho de cap phat !\n");
        return NULL;
    }
    new->element = add;
    new->next = NULL;
    new->prev = NULL;
    return new;
}

void insertAtHead(ElementType_dll newdata, listnode_dll *root, listnode_dll *tail){
    node* new = makeNewNode(newdata);
    if(*root == NULL){
        *root = new;
        *tail = new;
    }
    else{
        new->next = *root;
        (*root)->prev = new;
        *root = new;
    }
}

void insertAtTail(ElementType_dll newdata, listnode_dll *root, listnode_dll *tail){
    node* new = makeNewNode(newdata);
    if(*tail == NULL){
        *tail = new;
        *root = new;
    }
    else{
        (*tail)->next = new;
        new->prev = *tail;
        *tail = new;
    }
}

void insertAfterCurrent(ElementType_dll newdata, listnode_dll *root, listnode_dll *tail, listnode_dll *cur){
    node* new = makeNewNode(newdata);
    if(*root == NULL){
        *root = new;
        *tail = new;
    }
    else{
        new->next = (*cur)->next;
        if((*cur)->next != NULL)
            (*cur)->next->prev = new;
        else
            *tail = new;
        (*cur)->next = new;
        new->prev = *cur;
        *cur = new;
    }
}

void insertBeforeCurrent(ElementType_dll newdata, listnode_dll *root, listnode_dll *tail, listnode_dll *cur){
    node* new = makeNewNode(newdata);
    if(*root == NULL){
        *root = new;
        *tail = new;
    }
    else{
        new->next = *cur;
        if(*cur == *root){
            (*cur)->prev = new;
            *root = new;
        }
        else{
            new->prev = (*cur)->prev;
            (*cur)->prev->next = new;
        }
    }
}

void deleteFirstElement(listnode_dll *root, listnode_dll *tail){
    if(*root == NULL){
        printf("Loi con tro NULL\n");
        return;
    }
    else{
        node* tmp = *root;
        *root = (*root)->next;
        (*root)->prev = NULL;
        free(tmp);
    }
}

void deleteLastElement(listnode_dll *root, listnode_dll *tail){
    if(*tail == NULL){
        printf("Loi con tro NULL\n");
        return;
    }
    else{
        node *del = *tail;
        *tail = (*tail)->prev;
        if(*tail == NULL)
            *root = NULL;
        else
            (*tail)->next = NULL;
        free(del);
    }
}

void deleteCurrentElement(listnode_dll *root, listnode_dll *tail, listnode_dll *cur){
    if(*root==NULL){
        printf("Loi con tro NULL\n");
        return;
    }
    else{
        if(*cur == *root)
            deleteFirstElement(root, tail);
        else if(*cur == *tail)
            deleteLastElement(root, tail);
        else{
            node* p;
            (*cur)->prev->next = (*cur)->next;
            (*cur)->next->prev = (*cur)->prev;
            p = (*cur)->prev;
            free(*cur);
            (*cur) = p->next;
        }
    }
}

void insertAtPosition(ElementType_dll newdata, int position, listnode_dll *root, listnode_dll *tail){
    listnode_dll cur = *root;
    int i;
    if(position <= 0){
        insertAtHead(newdata, root, tail);
        return;
    }
 
    for(i=1; i < position; i++){
        if(cur->next != NULL)
            cur = cur->next;
    }
    insertAfterCurrent(newdata, root, tail, &cur);
}

void deleteAtPosition(int position, listnode_dll *root, listnode_dll *tail){
    int i;
    listnode_dll cur = *root;
    if(position <= 0){
        deleteFirstElement(root, tail);
        return;
    }

    for(i=1; i <= position; i++){
        if(cur->next != NULL)
            cur = cur->next;
    }
    deleteCurrentElement(root, tail, &cur);
}

void printNode(listnode_dll p){
    if(p == NULL){
        printf("Loi con tro NULL \n");
        return;
    }

    ElementType_dll tmp = p->element;
    printf("%d ", tmp);
}

void traverseListFromTail(listnode_dll tail){
    listnode_dll p;
    for(p = tail; p != NULL; p=p->prev)
        printNode(p);
}

void traverseListFromRoot(listnode_dll root){
    listnode_dll p;
    for(p = root; p != NULL; p=p->next)
        printNode(p);
}

void clean_up(listnode_dll *root){
    node* to_free;
    to_free = *root;
    while(to_free != NULL){
        *root = (*root)->next;
        free(to_free);
        to_free = *root;
    }
}

node* list_reverse(listnode_dll *root, listnode_dll *tail){
    node* temp;
    listnode_dll cur = *root;
    if(cur == NULL)
        return NULL;

    while(cur != NULL){
        temp = cur->prev;
        cur->prev = cur->next;
        cur->next = temp;
        cur = cur->prev;
    }

    if(temp != NULL)
        temp = temp->prev;
    return temp;
}