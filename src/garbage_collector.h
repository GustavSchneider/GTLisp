#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H
#include "data/types.h"

typedef struct garbage_node garbage_node;

struct garbage_node {
  Scheme_object* data;
  garbage_node* next;
};

void GB_REGISTER(Scheme_object*);
void GB_RUN(Scheme_object*);

#endif /*GARBAGE_COLLECTOR_H*/
