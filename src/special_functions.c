#include <assert.h>
#include <stdio.h>

#include "special_functions.h"
#include "scheme_utils.h"

#include "environment.h"
#include "data/constructor.h"
#include "data/prettyprint.h"
#include "read.h"
#include "eval.h"

#include "util/string.h"

#define INCLUDE_READ_BUFFER_SIZE 102400
 
Scheme_object* eval_quoted(Scheme_object* body, Scheme_object* env, Scheme_object* cont){
  return su_car(body);
}
 
Scheme_object* eval_if(Scheme_object* body, Scheme_object* env, Scheme_object* cont){
  Scheme_object* schemeRes = Scheme_nil;
  if (eval(su_car(body), env, NULL) == Scheme_false){
    Scheme_object* cddr =  su_cdr(su_cdr(body));
    if(su_is_pair(cddr))
      schemeRes = su_car(cddr);
  } else{
    schemeRes = su_car(su_cdr(body));
  }
  return eval(schemeRes, env, NULL);
} 

Scheme_object* eval_or(Scheme_object* body, Scheme_object* env, Scheme_object* cont){
  Scheme_object* result = Scheme_false;
  Scheme_object* currentPair = body;

  while(!su_is_null(currentPair) && result == Scheme_false){
    result = eval(su_car(currentPair), env, NULL);
    currentPair = su_cdr(currentPair);
  } 
  return result;
}

Scheme_object* eval_and(Scheme_object* body, Scheme_object* env, Scheme_object* cont){
  Scheme_object* result = Scheme_true;
  Scheme_object* currentPair = body;
  while(result != Scheme_false && su_is_pair(currentPair)){
    result = eval(su_car(currentPair), env, NULL);
    currentPair = su_cdr(currentPair);
  }
  
  return result;
}



Scheme_object* eval_define(Scheme_object* body, Scheme_object* env, Scheme_object* cont){
  Scheme_object* name = Scheme_void;
  Scheme_object* value = Scheme_void;
 
  if(su_is_pair(su_car(body))){
    Scheme_object* argsList = su_car(body);
    name = su_car(argsList);
    su_set_car(body, su_cdr(argsList)); // Change this, I'm hung over and just want to see results
    value = eval_lambda(body,env);
  } else {
    name = su_car(body);
    value = eval(su_car(su_cdr(body)), env, NULL);
  }

  assert(su_is_symbol(name));

  
  su_env_define(env, name, value);
  
  return Scheme_void;
}


Scheme_object* eval_lambda( Scheme_object* body, Scheme_object* env, Scheme_object* cont){
  return makeProcedure(body, env); 
}

Scheme_object* eval_set(Scheme_object* body, Scheme_object* env, Scheme_object* cont){
  Scheme_object* name = su_car(body);
  Scheme_object* value = eval(su_car(su_cdr(body)), env, NULL);
  assert(su_is_symbol(name));
  su_env_update(env, name, value);
  return Scheme_void;
}

Scheme_object* eval_include(Scheme_object* body, Scheme_object* env, Scheme_object* cont){
  Scheme_object* scm_filepath =  su_car(body);
  char* cstr = scm_filepath->data.string.str;
  FILE* fp = fopen( cstr, "r");
  
  if(fp == 0){
    fprintf(stderr,"failed to open file %s\n", cstr);
    exit(1336);
  }

  Scheme_object* filebody = makeEmptyPair();
  Scheme_object* currentPair = filebody;

  string_buffer buf;
  buf.size = 1024;
  buf.index = 0;
  buf.extrachar = 0;
  buf.buffer = malloc(sizeof(char) *  1024);
  
  bool eof = feof(fp);
  while(!su_is_null(currentPair) && !eof){
    
    su_set_car(currentPair, read_sexp(fp, &buf));

    eof = feof(fp);

    if(!eof && buf.extrachar == 0){
      char c;
      do{
	c = getc(fp);
	eof = feof(fp);
      } while(isWhiteSpace(c) && !eof);
      if(!isWhiteSpace(c)){
	buf.extrachar = c;
      }
    }
    
    if(eof){
      su_set_cdr(currentPair, Scheme_nil);
      currentPair = Scheme_nil;
      break;
    } else {
      Scheme_object* newPair  = makeEmptyPair();
      su_set_cdr(currentPair, newPair);
      currentPair =  newPair;
    }
  }

  eval_sequence(env, filebody, NULL);
  free(buf.buffer);
  fclose(fp);

  return Scheme_void;
}



Scheme_object* eval_define_macro(Scheme_object* define_body, Scheme_object* env, Scheme_object* cont){
  Scheme_object* name = su_car(su_car(define_body));
  su_set_car(define_body, su_cdr(su_car(define_body)));
  Scheme_object* args = su_car(define_body);
  
  Scheme_object* body = su_cdr(define_body);

  Scheme_object* macro = mallocSchemeObj();
  macro->type  = Scheme_MACRO;
  macro->data.macro.special = false;
  macro->data.macro.handler.scmproc = (Scheme_procedure){
    .scope = makeEnvironment(env),
    .args  = args,
    .body  = body
  };
  su_env_define(env, name, macro);
  return Scheme_void;
}


 Scheme_object* eval_env_add(Scheme_object* args, Scheme_object* env, Scheme_object* cont){
   su_env_define(env,su_car(args), su_car(su_cdr(args)));
   return Scheme_void;
 }


 Scheme_object* eval_env_new(Scheme_object* args, Scheme_object* env, Scheme_object* cont){
   Scheme_object* new_env = makeEnvironment(env);
   return new_env;
 }
