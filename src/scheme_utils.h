#ifndef SCHEME_UTILS_H
#define SCHEME_UTILS_H

#include <stdbool.h>
#include "data/types.h"

bool su_is_pair(Scheme_object*);
bool su_is_null(Scheme_object*);
bool su_is_symbol(Scheme_object*);


bool su_is_env(Scheme_object*);
bool su_is_kernel_env(Scheme_object*);
bool su_is_global_env(Scheme_object*);

Scheme_object* su_car(Scheme_object*);
Scheme_object* su_cdr(Scheme_object*);

Scheme_object* su_set_car(Scheme_object*, Scheme_object*);
Scheme_object* su_set_cdr(Scheme_object*, Scheme_object*);

Scheme_object* su_last(Scheme_object* list);

Scheme_object* su_cadr(Scheme_object*);
Scheme_object* su_caar(Scheme_object*);

Scheme_object* su_integer_to_float(Scheme_object*);
Scheme_object* su_float_to_integer(Scheme_object*);

void su_env_define(Scheme_object*, Scheme_object*,Scheme_object*);
void su_env_update(Scheme_object*, Scheme_object*,Scheme_object*);
Scheme_object* su_env_lookup(Scheme_object*, Scheme_object*);

Scheme_object* su_obj_at(Scheme_object*,int);
#endif
  
