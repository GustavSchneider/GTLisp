#include <stdio.h>
#include <assert.h>

#include "../src/primitive_proc.h"
#include "../src/data/types.h"
#include "../src/data/constructor.h"
#include "test_util.h"

void test_pp_float_to_integer() {
  Scheme_object* args = create_args_list(1, makeFloat(1.0));
  assert(pp_float_to_integer(args)->type == Scheme_INTEGER);
  assert(pp_float_to_integer(args)->data.integer == 1);
}

void test_pp_integer_add() {
  Scheme_object* list = makePair(makeFloat(1.0),
				 makePair(makeFloat(3.0),
					  makePair(makeFloat(5.0), Scheme_nil)));
  Scheme_object* args = create_args_list(1, list);
  assert(pp_integer_add(args)->data.real == 9.0);
  
  list = pp_range(makeInt(1), makeInt(4));
  args = create_args_list(1, list);
  assert(pp_integer_add(args)->type == Scheme_INTEGER);
  assert(pp_integer_add(args)->data.integer == 6);
}
void test_pp_integer_sub() {
  Scheme_object* list;
  Scheme_object* args;
  list = pp_range(makeInt(1), makeInt(4));
  args = create_args_list(1, list);
  assert(pp_integer_sub(args)->type == Scheme_INTEGER);
  assert(pp_integer_sub(args)->data.integer == -4);
  
  list = pp_range(makeInt(1), makeInt(2));
  args = create_args_list(1, list);
  assert(pp_integer_sub(args)->data.integer == -1);
}
void test_pp_is_integer_eq() {
  Scheme_object* o;
  Scheme_object* args;
  args = create_args_list(2, makeFloat(1.0), makeInt(1));
  o = pp_is_integer_eq(args);
  assert(o->data.scmBool == true);

  args = create_args_list(2, makeFloat(1.0), makeInt(2.0));
  o = pp_is_integer_eq(args);
  assert(o->data.scmBool == false);
}

int main(void) {
  puts("Running tests for primitive_proc.c");
  test_pp_float_to_integer();
  test_pp_integer_add();
  test_pp_integer_sub();
  //test_pp_is_integer_eq();
  puts("Ok!");
  return 0;
}
