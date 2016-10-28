#include "types.h"



Scheme_object Scheme_TRUE_OBJ =  {.type =  Scheme_BOOL, .data.scmBool = true};
Scheme_object Scheme_FALSE_OBJ = {.type =  Scheme_BOOL, .data.scmBool = false};
Scheme_object Scheme_NIL_OBJ = {.type = Scheme_NIL};
Scheme_object Scheme_VOID_OBJ = {.type = Scheme_VOID};

Scheme_object* get_exception_type(Scheme_object* exception) {
  return exception->data.exception.type;
}

char* get_exception_message(Scheme_object* exception) {
  return exception->data.exception.msg->data.string.str;
}
