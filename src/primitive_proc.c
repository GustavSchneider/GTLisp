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
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "primitive_proc.h"
#include "data/constructor.h"
#include "data/prettyprint.h"
#include "scheme_utils.h"

Scheme_object* pp_is_integer(Scheme_object* o) {
  if (o->data.pair.car->type == Scheme_INTEGER) {
    return Scheme_true;
  }
  return Scheme_false;
}
Scheme_object* pp_is_float(Scheme_object* o) {
  if (o->data.pair.car->type == Scheme_FLOAT) {
    return Scheme_true;
  }
  return Scheme_false;
}
Scheme_object* pp_is_string(Scheme_object* o) {
  if (o->data.pair.car->type == Scheme_STRING) {
    return Scheme_true;
  }
  return Scheme_false;
}
Scheme_object* pp_is_bool(Scheme_object* o) {<
  if (o->data.pair.car->type == Scheme_BOOL) {
    return Scheme_true;
  }
  return Scheme_false;
}
Scheme_object* pp_is_proc(Scheme_object* o) {
  if (o->data.pair.car->type == Scheme_PROCEDURE) {
    return Scheme_true;
  }
  return Scheme_false;
}
Scheme_object* pp_is_pair(Scheme_object* o) {
  if (o->data.pair.car->type == Scheme_PAIR) {
    return Scheme_true;
  }
  return Scheme_false;
}
Scheme_object* pp_is_symbol(Scheme_object* o) {
  if (o->data.pair.car->type == Scheme_SYMBOL) {
    return Scheme_true;
  }
  return Scheme_false;
}
Scheme_object* pp_is_cfun(Scheme_object* o) {
  if (o->data.pair.car->type == Scheme_CFUNC) {
    return Scheme_true;
  }
  return Scheme_false;
}
Scheme_object* pp_is_char(Scheme_object* o) {
  if (o->data.pair.car->type == Scheme_CHAR) {
    return Scheme_true;
  }
  return Scheme_false;
}

Scheme_object* pp_is_nil(Scheme_object* o) {
  if (o->data.pair.car->type == Scheme_NIL) {
    return Scheme_true;
  }
  return Scheme_false;
}

int is_number(Scheme_object* o) {
  switch (o->type) {
  case Scheme_INTEGER:
  case Scheme_FLOAT: {
    return 1;
    break;
  }
  default: {
    return 0;
  }
  }
}

Scheme_object* pp_integer_to_float(Scheme_object* args) {
  /* TODO:  implement error check*/
  return su_integer_to_float(su_car(args));
}

Scheme_object* pp_float_to_integer(Scheme_object* args) {
  /* TODO:  implement error check*/
  return su_float_to_integer(su_car(args));
}

Scheme_object* pp_is_integer_eq(Scheme_object* args) {
  Scheme_object* a = su_car(args);
  Scheme_object* b = su_car(su_cdr(args));
  if (!is_number(a) && !is_number(b)) {
    return Scheme_false; //this should be an exception
  }
  if ((a->type == Scheme_INTEGER ? (double) a->data.integer : a->data.real) ==
      (b->type == Scheme_INTEGER ? (double) b->data.integer : b->data.real)) {
    return Scheme_true;
  }
  return Scheme_false;
}
Scheme_object* pp_range(Scheme_object* start, Scheme_object* end) {
  if (start->type != Scheme_INTEGER && end->type != Scheme_INTEGER) {
    return makeException(Scheme_nil, "range takes two integers as arguments!");
  }
  if (start->data.integer == end->data.integer) {
    return Scheme_nil;
  }
  return makePair(start, pp_range(makeInt(start->data.integer + 1), end));
}

double get_numeric(Scheme_object* o) {
  if (o->type == Scheme_FLOAT) {
    return o->data.real;
  } else if (o->type == Scheme_INTEGER) {
    return (double) o->data.integer;
  } else {
    fprintf(stderr, "wrong type");
    exit(1336);
  }
}

Scheme_object* pp_integer_div(Scheme_object* list) {
  Scheme_object* nominator = su_obj_at(list, 0);
  double nom = get_numeric(nominator);			       	
  if (su_cdr(list) == Scheme_nil) {
    return makeFloat(1.0 / nom);
  }

  Scheme_object* current = su_cdr(list);
  while (current != Scheme_nil) {
    nom = nom / get_numeric(su_car(current));
    current = su_cdr(current);
  }

  return makeFloat(nom);
}

Scheme_object* pp_integer_mult(Scheme_object* list) {
  if (list->type != Scheme_PAIR) {
    fprintf(stderr, "argument to pp_integer_mult is not an pair");
    exit(1);
  }
  bool should_be_float = false;
  double sum = 1.0;

  Scheme_object* current = list;
  while (current->type != Scheme_NIL) {
    if (su_car(current)->type == Scheme_INTEGER) {
      sum = sum * (double) su_car(current)->data.integer;
    } else if (su_car(current)->type == Scheme_FLOAT) {
      should_be_float = true;
      sum = sum * su_car(current)->data.real;
    } else {
      return makeException(Scheme_nil, "pp_integer_mult only handles numeric objects");
      exit(1);
    }
    current = su_cdr(current);
  }
  
  if (should_be_float) {
    return makeFloat(sum);
  }
  return makeInt((long int) sum);
}

Scheme_object* pp_integer_sub(Scheme_object* list) {
  if (list->type != Scheme_PAIR) {
    fprintf(stderr, "argument to pp_integer_sub is not an pair");
    exit(1);
  }

  double sum = 0.0;
  bool should_be_float = false;
  if (su_car(list)->type == Scheme_FLOAT) {
    should_be_float = true;
    sum = su_car(list)->data.real;
  } else if (su_car(list)->type == Scheme_INTEGER) {
    sum = (double) su_car(list)->data.integer;
  } else {
    fprintf(stderr, "wrong type");
    exit(1);
    //Hanlde error
  }
  if (su_cdr(list)->type == Scheme_NIL) {
    if (should_be_float)
      return makeFloat(-sum);
    return makeInt(-sum);
  }
  Scheme_object* current = su_cdr(list);
  while (current->type != Scheme_NIL) {
    if (su_car(current)->type == Scheme_INTEGER) {
      sum -= (double)su_car(current)->data.integer;
    } else if (su_car(current)->type == Scheme_FLOAT) {
      should_be_float = true;
      sum -= su_car(current)->data.real;
    } else {
      //Handle errors
      exit(1);
    }
    current = su_cdr(current);
  }

  if (should_be_float) {
    return makeFloat(sum);
  }
  return makeInt((long int) sum);
}

Scheme_object* pp_integer_add(Scheme_object* list) {
  if (list->type != Scheme_PAIR) {
    fprintf(stderr, "argument to pp_integer_add is not an pair");
    exit(1);
  }
  bool should_be_float = false;
  Scheme_object* tot = makeFloat(0.0);
  Scheme_object* current = list;
  while (current->type != Scheme_NIL) {
    if (current->data.pair.car->type == Scheme_FLOAT) {
      should_be_float = true;
      tot->data.real += current->data.pair.car->data.real;
    } else if (current->data.pair.car->type == Scheme_INTEGER) {
      tot->data.real += (double)current->data.pair.car->data.integer;
    } else {
      /* TODO: Error handling */
      #ifdef DEBUG
      fprintf(stderr, "argument to pp_integer_add is not an numeric object");
      exit(2);
      #endif
    }
    current = current->data.pair.cdr;
  }
  if (should_be_float)
    return tot;
  return su_float_to_integer(tot);
}

Scheme_object* pp_car(Scheme_object* pair) {
  return pair->data.pair.car->data.pair.car;
}

Scheme_object* pp_cdr(Scheme_object* pair) {
  return pair->data.pair.car->data.pair.cdr;
}

Scheme_object* pp_cons(Scheme_object* args){
  return  makePair(su_car(args), su_car(su_cdr(args)));
}


Scheme_object* pp_quit(Scheme_object* args) {
  exit(0);
}

Scheme_object* pp_integer_less(Scheme_object* args) {
  Scheme_object* a = su_car(args);
  Scheme_object* b = su_car(su_cdr(args));
  if ((a->type == Scheme_INTEGER ? (double) a->data.integer : a->data.real) <
      (b->type == Scheme_INTEGER ? (double) b->data.integer : b->data.real)) {
    return Scheme_true;
  }
  return Scheme_false;
}

Scheme_object* pp_integer_greater(Scheme_object* args) {
  Scheme_object* a = su_car(args);
  Scheme_object* b = su_car(su_cdr(args));
  if ((a->type == Scheme_INTEGER ? (double) a->data.integer : a->data.real) >
      (b->type == Scheme_INTEGER ? (double) b->data.integer : b->data.real)) {
    return Scheme_true;
  }
  return Scheme_false;
}

Scheme_object* pp_mod(Scheme_object* args) {
  Scheme_object* a = su_obj_at(args, 0);
  Scheme_object* b = su_obj_at(args, 1);
  if (a->type != Scheme_INTEGER || b->type != Scheme_INTEGER) {
    fprintf(stderr, "Wrong type to pp_mod\n");
    exit(1);
  }
  return makeInt(a->data.integer % b->data.integer);
}

Scheme_object* pp_eq(Scheme_object* args) {
  Scheme_object* a = su_car(args);
  Scheme_object* b = su_car(su_cdr(args));

  //the relevant pointer when the argument is a symbol is not the pointer
  //to the scheme_object
  if (a->type == Scheme_SYMBOL && b->type == Scheme_SYMBOL &&
      a->data.symbol == b->data.symbol) {
    return Scheme_true;
  } else if (a == b) {
    return Scheme_true;
  }
  return Scheme_false;
}

Scheme_object* pp_pretty_print(Scheme_object* args) {
  prettyprint(su_car(args));
  printf("\n");
  return Scheme_void;
}


Scheme_object* pp_string_to_symbol(Scheme_object* args){
  assert(pp_is_string(args));
  Scheme_object* str = su_car(args);
  return makeSymbol(str->data.string.str, str->data.string.len);
}
Scheme_object* pp_symbol_to_string(Scheme_object* args){
  Scheme_string symstr = su_car(args)->data.symbol->str;
  return makeString(symstr.str,symstr.len);
}

Scheme_object* pp_sleep(Scheme_object* args) {
  time_t  sec_time = su_obj_at(args, 0)->data.integer;
  long nano_time = 0;
  if(su_cdr(args) != Scheme_nil){
    nano_time = su_car(su_cdr(args))->data.integer;
  }
  nanosleep((const struct timespec[]){{sec_time, nano_time}}, NULL);
  return Scheme_void;
}

Scheme_object* pp_random(Scheme_object* args) {
  srand(time(NULL));
  return makeInt(rand());
}
