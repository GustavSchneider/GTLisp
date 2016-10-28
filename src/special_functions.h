#ifndef SEPCIAL_FUNCTIONS_H
#define SEPCIAL_FUNCTIONS_H

#include "data/types.h"


Scheme_object* eval_if(Scheme_object* body, Scheme_object* env, Scheme_object* cont);
Scheme_object* eval_or(Scheme_object* body, Scheme_object* env, Scheme_object* cont);
Scheme_object* eval_and( Scheme_object* , Scheme_object*, Scheme_object* env);
Scheme_object* eval_define(Scheme_object* body, Scheme_object* env, Scheme_object* cont);
Scheme_object* eval_define_macro(Scheme_object*, Scheme_object*, Scheme_object* env);
Scheme_object* eval_define_primitive(Scheme_object*, Scheme_object*, Scheme_object* env);
Scheme_object* eval_lambda(Scheme_object* body, Scheme_object* env, Scheme_object* cont);
Scheme_object* eval_set(Scheme_object* body, Scheme_object* env, Scheme_object* cont);

Scheme_object* eval_quoted(Scheme_object*, Scheme_object*, Scheme_object*);
Scheme_object* eval_include(Scheme_object*, Scheme_object*, Scheme_object*); 




#endif  
