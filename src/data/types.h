#ifndef DATATYPES_H
#define DATATYPES_H

#include <stdbool.h>
#include <string.h>

typedef enum{
  Scheme_NIL = 0,
  Scheme_VOID,
  Scheme_BOOL,
  Scheme_SYMBOL,
  Scheme_PAIR,
  Scheme_PROCEDURE,
  Scheme_CFUNC,
  Scheme_POINTER,
  Scheme_INTEGER,
  Scheme_FLOAT,
  Scheme_FRACTION,
  Scheme_CHAR,
  Scheme_STRING,
  Scheme_VECTOR,
  Scheme_PORT,
  Scheme_ENV,
  Scheme_CONT,
  
  Scheme_CPTR,

  Scheme_EXCEPTION,

  // These are not to be used in actual computations
  Scheme_MACRO,
  Scheme_SYNTATIC_KEYWORD
} Scheme_type;


typedef struct Scheme_object Scheme_object;
typedef struct Scheme_pair Scheme_pair;
typedef struct Scheme_symbol Scheme_symbol;

typedef struct Scheme_pcont Scheme_pcont;

/* Enviroment definition ========================*/
typedef struct bucket bucket;
typedef struct environment environment;

struct environment {
  Scheme_object* parent;
  bucket** map;
  unsigned long items;
  unsigned long size;
}; 
 
struct bucket {
  Scheme_symbol* key;
  Scheme_object* data;
};
/*=================================================*/




/*
 * Symbol
 * The symbol should be identified by a pointer to something (like something tha containts its string value) 
 */

/*
 * Pair
 * Pair is simply two pointers to some other kind of data 
 */


/*
 * Lambda
 * I do not know yet but my best guess would be that lambdas are a pointer to it's scope and 
 * a pointer to the list that contains the sexp to evauluate.
 */
typedef struct{
  Scheme_object* scope;
  Scheme_object* args;
  Scheme_object* body;
} Scheme_procedure;

/*
 * CFUNC 
 * the cfunc is a wildcard way to call a c function. 
 * it contains a number of arguments and a pointer to the actual function
 * thus when the c function should be called there are a ton of pre made cfunctions that can apply arguments to the function.
 * WARNING this is most probably retarded and WILL Change! 
 */

typedef struct{
  int nArgs;
  Scheme_object* (*funPtr)(Scheme_object*);
} Scheme_cfunc;

/*
 * String
 * Strings are different from c string in that they are NOT null terminated, the length is known all the time
 */

typedef struct{
  size_t len;
  char* str;
} Scheme_string;

struct Scheme_symbol{
  Scheme_string str;
  unsigned long hash;
};

struct Scheme_pair{
  Scheme_object* car;
  Scheme_object* cdr;
};

typedef struct{
  Scheme_object* type;
  Scheme_object* msg;
} Scheme_exception;

Scheme_object* get_exception_type(Scheme_object*);
char* get_exception_message(Scheme_object*);

typedef struct{
  bool special;
  union{
    Scheme_procedure scmproc;
    Scheme_object* (*specproc)(Scheme_object*,Scheme_object*);
  } handler; 
} Scheme_macro;


typedef union{
  Scheme_symbol*     symbol;
  Scheme_pair        pair;
  Scheme_procedure   proc;
  Scheme_cfunc       cfunc;
  Scheme_string      string;
  long int           integer;
  double             real;
  bool               scmBool;
  void*              cptr;
  Scheme_exception   exception;
  Scheme_macro       macro;
  environment*       env;
  Scheme_pcont*      cont;
} Scheme_data;

 
struct Scheme_object{
  int            marked;
  Scheme_type      type;
  Scheme_data      data;
};
/*
 * Nil  
 * The Nil object doesn't require any extra data 
 * And as you can see here might as well just  be declared once
 */
Scheme_object Scheme_NIL_OBJ;
#define Scheme_nil (&Scheme_NIL_OBJ)

Scheme_object Scheme_TRUE_OBJ;
Scheme_object Scheme_FALSE_OBJ;

#define Scheme_true (&Scheme_TRUE_OBJ)
#define Scheme_false (&Scheme_FALSE_OBJ)

Scheme_object Scheme_VOID_OBJ;
#define Scheme_void (&Scheme_VOID_OBJ)

struct Scheme_pcont {
  Scheme_object** valptr; // pointer to where to put result, if null result is thrown away
  Scheme_object* next;  // what to evaluate after continuation pop
  Scheme_pcont* cont; //parent
};



#endif /* DATATYPES_H */
