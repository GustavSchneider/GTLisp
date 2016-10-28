/* <gt-lisp>
 * Copyright (C) <2016>  <Gustav Nelson Schneider & Tomas Möre>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * <gt-lisp> Copyright (C) <2016>  <Gustav Nelson Schneider & Tomas Möre>
 * This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
 * This is free software, and you are welcome to redistribute it
 * under certain conditions; type `show c' for details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "sexp_parser.h"
#include "data/types.h"
#include "data/constructor.h"
#include "data/prettyprint.h"

#include "environment.h"
#include "eval.h"
#include "apply.h"

#include "macro.h"

#include "primitive_proc.h"
#include "scheme_utils.h" 
 
Scheme_object* map_eval(Scheme_object* env, Scheme_object* list);
Scheme_object* eval_special(Scheme_symbol*, Scheme_object*, environment*);
			   
Scheme_object* eval(Scheme_object* exp, Scheme_object* env, Scheme_object* cont) {
  Scheme_object* result;
  while(true){
    switch (exp->type) {
    case Scheme_PAIR: {
      // This shouldn't be a recursive eval for macro reasonsc
      Scheme_object* car = eval(su_car(exp), env, NULL);
    
      switch(car->type){
      case Scheme_CFUNC:
      case Scheme_PROCEDURE:
	result = apply(car, map_eval(env, su_cdr(exp)));
	break;
      case Scheme_MACRO: {
	Scheme_object* macro_res =  macro_eval(car, exp, env); 
	if(car->data.macro.special){
	  result = macro_res; 
	} else {
	  result =  eval(macro_res, env, NULL);
	}
	break;
      }
      case Scheme_CONT:{
	Scheme_object* result_ptr = *(car->data.cont->valptr);
        if(result_ptr != NULL)
	  result_ptr = su_last(eval(su_cdr(exp), env, NULL));
	
	continue;
	break;
      }
      default:
	prettyprint(exp);
	fprintf(stderr, "In capitalist america object runns you\n");
	exit(1336);
	break;
      }
      break;
    }
    case Scheme_SYMBOL: {
      Scheme_object* val  = su_env_lookup(env, exp);
      if(val == 0) 
	val = Scheme_void;
      return val;
      break;
    }
    default: {
      result =  exp;
      break;
    }
    }

    
  return result;
  }
}

bool should_tail_call(Scheme_object* env, Scheme_object* cp, Scheme_object* caller){
  if(!su_is_pair(su_car(cp)))
    return false;
  Scheme_object* proc = eval(su_car(su_car(cp)), env, NULL);
  return proc == caller;
} 

Scheme_object* eval_sequence(Scheme_object* env, Scheme_object* body, Scheme_object* caller){
  Scheme_object* curr_pair = body;
  Scheme_object* last_val = Scheme_nil;

  while(curr_pair != Scheme_nil){
    
    if(su_cdr(curr_pair) == Scheme_nil && should_tail_call(env,curr_pair,caller)){
      bind_arguments(env, caller->data.proc.args, map_eval(env, su_cdr(curr_pair)));
      curr_pair = caller->data.proc.body;
      continue;
    }
    
    last_val = eval(su_car(curr_pair), env, NULL);
    curr_pair = su_cdr(curr_pair);

    if(!(su_is_pair(curr_pair) || su_is_null(curr_pair))){
      fprintf(stderr, "Wtf");
      exit(1336);
    }
  }
  return last_val;
}



Scheme_object* map_eval(Scheme_object* env, Scheme_object* list){
  assert(su_is_pair(list) || su_is_null(list));
  
  if(su_is_null(list))
    return list;

  Scheme_object* evaledList = makeEmptyPair();
  Scheme_object* evaledCurrent = evaledList;
  Scheme_object* codeCurrent = list;
  
  while(su_is_pair(codeCurrent)){
    su_set_car(evaledCurrent, eval(su_car(codeCurrent), env, NULL));

    if(!su_is_null(su_cdr(codeCurrent))){
      Scheme_object* newPair = makeEmptyPair();
      su_set_cdr(evaledCurrent, newPair);
      evaledCurrent = newPair;
    }   
    codeCurrent = su_cdr(codeCurrent);
  }

  su_set_cdr(evaledCurrent, Scheme_nil);
  
  return evaledList;
}
