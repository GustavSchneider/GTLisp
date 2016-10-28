#ifndef APPLY_H
#define APPLY_H

#include <stdio.h>
#include <stdlib.h>

#include "sexp_parser.h"
#include "data/types.h"

Scheme_object* apply(Scheme_object*, Scheme_object*);
void bind_arguments(Scheme_object*, Scheme_object*, Scheme_object*);

#endif /* APPLY_H */







