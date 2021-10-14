#ifndef CSTACK_H_
#define CSTACK_H_

#include <stddef.h>

#include "libfdr/dllist.h"
#include "libfdr/jval.h"

typedef Dllist Stack;

Stack stack_create();
void stack_push(Stack, Jval);
int stack_pop(Stack);
Jval stack_top(Stack);
int stack_empty(Stack);
void stack_free(Stack);
size_t stack_size(Stack);

#define stack_traverse(ptr, list) \
  for (ptr = list->flink; ptr != list; ptr = ptr->flink)
#define stack_rtraverse(ptr, list) \
  for (ptr = list->blink; ptr != list; ptr = ptr->blink)
#endif  // CSTACK_H_