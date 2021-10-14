#ifndef CQUEUE_H_
#define CQUEUE_H_

#include <stddef.h>

#include "libfdr/dllist.h"
#include "libfdr/jval.h"

typedef Dllist Queue;

Queue queue_create();
void enQueue(Queue, Jval);
int deQueue(Queue);
Jval queue_front(Queue);
Jval queue_rear(Queue);
int queue_empty(Queue);
void queue_free(Queue);
size_t queue_size(Queue);
#define queue_traverse(ptr, list) \
  for (ptr = list->blink; ptr != list; ptr = ptr->blink)
#define queue_rtraverse(ptr, list) \
  for (ptr = list->flink; ptr != list; ptr = ptr->flink)
#endif  // CQUEUE_H_