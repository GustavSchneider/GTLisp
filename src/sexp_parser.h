#ifndef SEXP_PARSER_H
#define SEXP_PARSER_H

#include "data/types.h"


typedef enum{
  SEXP_PARSE_SUCCESS = 0,
  SEXP_PARSE_ERROR_UNKOWN,
  SEXP_PARSE_ERROR_IMPROPERLY_PLACED_DOT,
  SEXP_PARSE_ERROR_DATUM_EXPECTED,
  SEXP_PARSE_ERROR_DATUM_OR_EOF_EXPCETED,
  SEXP_PARSE_ERROR_INCOMPLETE_FORM
  
} sexp_parse_status; 
 

typedef struct{
  sexp_parse_status   status;
  char*               remainder;
  Scheme_object*      scmObj;
} sexp_parse_result;

/*
  sexp_parse is the default parsing algoritm it parses One sexp and no more, if this m
 */
sexp_parse_result sexp_parse(char*);

/*
  Sexp multi parse assumes that the input string is a "list of sexps" but with an invissible "list" tag around it
  such that 
  "(define a 1) (define b 2)"
  equals 
  "'((define a 1) (define b 2))"
 */
sexp_parse_result* sexp_mutli_parse(char*);




#endif /*  SEXP_PARSER_H */
