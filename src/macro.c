#include "macro.h"

#include "scheme_utils.h"
#include "apply.h"
#include "data/types.h"
#include "data/prettyprint.h"

Scheme_object*  macro_eval(Scheme_object* macro, Scheme_object* container, Scheme_object* env){
  if(macro->data.macro.special){
    return macro->data.macro.handler.specproc(su_cdr(container), env);
  } else{
    return apply(macro, su_cdr(container));
  } 
}
 
 
 
 
