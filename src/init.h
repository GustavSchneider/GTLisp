#ifndef INIT_H
#define INIT_H

#include "data/types.h"
#include "symbol_table.h"
#include "read.h"

typedef struct{
  Scheme_object* env;
  string_buffer* readbuffer;
} init_data;

init_data init();

#endif
