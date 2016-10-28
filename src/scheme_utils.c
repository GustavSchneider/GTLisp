
#include <assert.h>
#include <stdbool.h>
#include "data/types.h"
#include "data/constructor.h"
#include "environment.h"

bool su_is_pair(Scheme_object* o){
  return o->type == Scheme_PAIR;
}
bool su_is_null(Scheme_object* o){
  return o->type == Scheme_NIL;
}
bool su_is_symbol(Scheme_object* o){
  return o->type == Scheme_SYMBOL;
}

bool su_is_env(Scheme_object* env){
  return env->type == Scheme_ENV;
}

bool su_is_kernel_env(Scheme_object* env){
  return su_is_env(env) && env->data.env->parent == 0;
}

bool su_is_global_env(Scheme_object* env){
  if(su_is_kernel_env(env)){
    return false;
  } else {
    return su_is_env(env) && (env->data.env->parent->data.env->parent == 0);
  }
}

Scheme_object* su_car(Scheme_object* o){
  assert(su_is_pair(o));
  return o->data.pair.car;
}
Scheme_object* su_cdr(Scheme_object* o){
    assert(su_is_pair(o));
  return o->data.pair.cdr;
}

void su_set_car(Scheme_object* p, Scheme_object* o){
  assert(su_is_pair(p));
  p->data.pair.car = o;
}

void su_set_cdr(Scheme_object* p , Scheme_object* o){
  assert(su_is_pair(p));
  p->data.pair.cdr = o;
}

Scheme_object* su_last(Scheme_object* list){
  assert(list != Scheme_nil);
  Scheme_object* cp = list;
  while(su_cdr(cp) != Scheme_nil){
    cp = su_cdr(cp);
  }
  return su_car(cp);
}

Scheme_object* su_integer_to_float(Scheme_object* o) {
  assert(o->type == Scheme_INTEGER);
  return makeFloat((double) o->data.integer);
}

Scheme_object* su_float_to_integer(Scheme_object* o) {
  assert(o->type == Scheme_FLOAT);
  return makeInt((long int) o->data.real);
}


void su_env_define(Scheme_object* env, Scheme_object* key, Scheme_object* val){
  env_define(env->data.env, key->data.symbol, val);
}
void su_env_update(Scheme_object* env, Scheme_object* key, Scheme_object* val){
  env_set(env->data.env, key->data.symbol, val);
}

Scheme_object* su_env_lookup(Scheme_object* env, Scheme_object* key){
  return env_lookup(env->data.env, key->data.symbol);
}



Scheme_object* su_obj_at(Scheme_object* list, int index){
  Scheme_object* cp = list;
  for(int i = 0; i < index; i++){
    cp = su_cdr(cp);
    assert(cp != Scheme_nil);
  }
  return su_car(cp);
};
