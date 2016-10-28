#ifndef DATACONSTRUCTOR_H
#define DATACONSTRUCTOR_H

#include "types.h"
#include "../symbol_table.h"

Scheme_object* mallocSchemeObj();
Scheme_object* makePair(Scheme_object*,Scheme_object*);
Scheme_object* makeEmptyPair();
// Quoted pairs are pairs where car is the symbol "QUOTED" for the eval function 
Scheme_object* makeQuoted(Scheme_object*);
Scheme_object* makeInt(int);
Scheme_object* makeFloat(double);
Scheme_object* makeLambda(void*, void*);
// Creates a string from a c string and a length
Scheme_object* makeSymbol(char*, size_t);
// makes a string from start to null
Scheme_object* makeSymbolFromCString(char*);
// currently allocates a new string for the value
Scheme_object* makeString(char*, size_t);
Scheme_object* makeCFunc(void*, int);
Scheme_object* makeProcedure(Scheme_object*, Scheme_object*);
Scheme_object* makeEnvironment(Scheme_object*);

Scheme_object* makeException(Scheme_object*, char*);

#endif
