#ifndef READSEXP_H
#define READSEXP_H


#include <stdio.h>
#include <stdlib.h>

#include "sexp_parser.h"
#include "data/types.h"
#include "symbol_table.h"

typedef struct{
  char* buffer;
  int   size;
  int   index;
  char  extrachar;
  
} string_buffer;

Scheme_object* read_sexp(FILE*, string_buffer*);



#endif
