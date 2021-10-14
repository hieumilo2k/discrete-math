#include "stack_dllist.h"

Stack stack_create()
{
    return new_dllist();
}


void stack_push(Stack q, Jval val)
{
   return dll_insert_b(q->flink, val);
}

int stack_pop(Stack q)
{
    if(!stack_empty(q))
    {	
        dll_delete_node(q->flink);
        return 1;
    }
    else return 0;
}

Jval stack_top(Stack q)
{
   return dll_val((q->flink));
}


int stack_empty(Stack q)
{
    return dll_empty(q);
}
void stack_free(Stack q)
{
    free_dllist(q);
}

size_t stack_size(Stack q)
{
    Stack ptr;
    size_t dem = 0;
    dll_traverse(ptr, q)
        dem++;
    return dem;
}