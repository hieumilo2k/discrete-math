#include "queue_dllist.h"

Queue queue_create()
{
    return new_dllist();
}


void enQueue(Queue q, Jval val)
{
   return dll_insert_b(q->flink, val);
}

int deQueue(Queue q)
{
    if(!queue_empty(q))
    {	
        dll_delete_node(q->blink);
        return 1;
    }
    else return 0;
}

Jval queue_front(Queue q)
{
    return dll_val(q->blink);
}

Jval queue_rear(Queue q)
{
   return dll_val((q->flink));
}


int queue_empty(Queue q)
{
    return dll_empty(q);
}
void queue_free(Queue q)
{
    free_dllist(q);
}

size_t queue_size(Queue q)
{
    Queue ptr;
    size_t dem = 0;
    dll_traverse(ptr, q)
    dem++;
    return dem;
}