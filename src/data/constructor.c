
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "constructor.h"
#include "types.h"
#include "prettyprint.h"
#include "../symbol_table.h"
#include "../environment.h"
#include "../garbage_collector.h"
#include "../scheme_utils.h"


// Allocates prim data, probably should have somem more features for garbage collection
Scheme_object* mallocSchemeObj(){

  Scheme_object* so = calloc(sizeof(Scheme_object), 1);
  if (so == NULL) {
    fprintf(stderr, "mallco failed to allocate memory\n");
    exit(1336);
  }
  GB_REGISTER(so);
  so->marked = 0;
  return so;
};


Scheme_object* makeString(char* cString, size_t length){
  Scheme_object* newString = mallocSchemeObj();
  char* string = malloc(sizeof(char) * length + 1);
  strncpy(string, cString, length);
  string[length] = 0;
  newString->type = Scheme_STRING;
  newString->data.string.len = length;
  newString->data.string.str = string;
  return newString;
};


Scheme_object* makePair(Scheme_object* car, Scheme_object* cdr){
  Scheme_object* newPair = makeEmptyPair();
  newPair->data.pair.car = car;
  newPair->data.pair.cdr = cdr;
  return newPair;
}

Scheme_object* makeEmptyPair(){
  Scheme_object* newPair = mallocSchemeObj();
  newPair->type = Scheme_PAIR;
  newPair->data.pair.car = NULL;
  newPair->data.pair.cdr = NULL;
  return newPair;
}


Scheme_object* makeQuoted(Scheme_object* cdr){
  Scheme_object* newPair = makeEmptyPair();
  newPair->data.pair.car = makeSymbol("quoted", 6);
  newPair->data.pair.cdr = makePair(cdr, Scheme_nil); 
  return newPair;
}


Scheme_object* makeSymbol(char* str, size_t len){
  Scheme_object* sym = mallocSchemeObj();
  sym->type = Scheme_SYMBOL;
  sym->data.symbol = st_add(str, len); 
  return  sym;
}

Scheme_object* makeSymbolFromCString(char* s){
  return makeSymbol(s, strlen(s));
};

Scheme_object* makeInt(int n){
  Scheme_object* newInt =  mallocSchemeObj();
  newInt->type = Scheme_INTEGER;
  newInt->data.integer = n;
  return newInt;
}

 
Scheme_object* makeFloat(double n){
  Scheme_object* newDouble =  mallocSchemeObj();
  newDouble->type = Scheme_FLOAT;
  newDouble->data.real = n;
  return newDouble;
}
 


Scheme_object* makeCFunc(void* funPtr, int n){
  Scheme_object* newCFunc = mallocSchemeObj();
  newCFunc->type = Scheme_CFUNC;
  newCFunc->data.cfunc.nArgs  = n;
  newCFunc->data.cfunc.funPtr = funPtr;
  return newCFunc;

}

Scheme_object* makeEnvironment(Scheme_object* parent) {
  Scheme_object* env = mallocSchemeObj();
  env->type = Scheme_ENV;
  env->data.env = env_new(parent);
  return env;
}

Scheme_object* makeProcedure(Scheme_object* body, Scheme_object* env){
  Scheme_object* proc = mallocSchemeObj();
  proc->type = Scheme_PROCEDURE;
  proc->data.proc.scope = env;
  proc->data.proc.args = su_car(body);
  proc->data.proc.body = su_cdr(body);
  return proc;
}

Scheme_object* makeException(Scheme_object* type, char* msg) {
  Scheme_object* exp = mallocSchemeObj();
  exp->type = Scheme_EXCEPTION;
  exp->data.exception.type = Scheme_nil;
  exp->data.exception.msg = makeString(msg, strlen(msg));
  return exp;
}

Scheme_object* makeCont(Scheme_object* valptr, Scheme_object* next, Scheme_object* cont){
  Scheme_object* new_cont = mallocSchemeObj();
  new_cont->type = Scheme_CONT;
  new_cont->data.cont->valptr = valptr;
  new_cont->data.cont->next = next;
  new_cont->data.cont->cont = cont;
  return new_cont;
}
