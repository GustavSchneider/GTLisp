#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "eval.h"
#include "sexp_parser.h"
#include "data/types.h"
#include "apply.h"
#include "environment.h"
#include "scheme_utils.h"
#include "data/constructor.h"
#include "data/prettyprint.h"

void bind_arguments(Scheme_object* env, Scheme_object* args, Scheme_object* values) {
  switch (args->type) {
  
  case Scheme_NIL: {
    if (values->type == Scheme_NIL) {
      
      return;
    } else {
      fprintf(stderr, "Too many arguments to function\n");
      exit(1336);
    } 
  }
  case Scheme_SYMBOL: {
    env_define(env->data.env, args->data.symbol, values);
    break;
  }
  case Scheme_PAIR: {
    if (su_is_symbol(su_car(args)) && su_is_pair(values)) {
      su_env_define(env, su_car(args), su_car(values));
      return bind_arguments(env, su_cdr(args), su_cdr(values));
    } else{
      fprintf(stderr, "Not enought arguments\n");
      exit(1336);
    }
    break;
  }
  default: {
    printf("Identifier expected\n");
    exit(1336);
  }     
  }
}
 
Scheme_object* apply(Scheme_object* proc, Scheme_object* values) {
  Scheme_object* result = Scheme_void;
  switch(proc->type){
  case Scheme_MACRO:
  case Scheme_PROCEDURE:{
    Scheme_procedure sproc;
    if(proc->type == Scheme_PROCEDURE)
      sproc = proc->data.proc;
    else
      sproc = proc->data.macro.handler.scmproc;
    
    Scheme_object* new_env =  makeEnvironment(sproc.scope);
 
    bind_arguments(new_env, sproc.args, values);
    result = eval_sequence(new_env, sproc.body, proc);
    break;
  }
  case Scheme_CFUNC: {
    result = proc->data.cfunc.funPtr(values);
    break;
  }
  default:
    printf("First argument to apply needs to be a procedure");
    exit(1336);
    break;
  }
  return result;
}


 
