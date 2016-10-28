#include <stdlib.h>
#include <stdio.h>

#include "init.h"
#include "data/constructor.h"
#include "data/types.h"
#include "symbol_table.h"
#include "read.h"

#include "environment.h"

#include "primitive_proc.h"
#include "special_functions.h"

#include "scheme_utils.h"

#include "bindings/graphics.h"

#define READ_BUFFER_SIZE 10485776

void set_global(Scheme_object* env, Scheme_object* o, char* name) {
  su_env_define(env, makeSymbolFromCString(name), o);
  
}

void register_cfun(Scheme_object* env, void* func, char* symName, int argc) {
  Scheme_object* fun = makeCFunc((void*)func, argc);
  su_env_define(env, makeSymbolFromCString(symName), fun);
} 

Scheme_object* init_kernel_environment() {
  Scheme_object* kernel = makeEnvironment(NULL);
  register_cfun(kernel, pp_integer_add,  "+", -1);
  register_cfun(kernel, pp_integer_sub,  "-", -1);
  register_cfun(kernel, pp_integer_mult, "*", -1);
  register_cfun(kernel, pp_integer_div,  "/", -1);
  register_cfun(kernel, pp_mod,  "mod", 2);
  
  register_cfun(kernel, pp_is_float,   "float?", 1);
  register_cfun(kernel, pp_is_integer, "integer?", 1);
  register_cfun(kernel, pp_is_string,  "string?", 1);
  register_cfun(kernel, pp_is_bool,    "bool?", 1);
  register_cfun(kernel, pp_is_proc,    "process?", 1);
  register_cfun(kernel, pp_is_pair,    "pair?", 1);
  register_cfun(kernel, pp_is_symbol,  "symbol?", 1);
  register_cfun(kernel, pp_is_nil,     "nil?", 1);

  register_cfun(kernel, pp_integer_to_float, "int->float", 1);
  register_cfun(kernel, pp_float_to_integer, "float->int", 1);

  register_cfun(kernel, pp_is_integer_eq,   "integer-equal?", 2);
  register_cfun(kernel, pp_integer_greater, "integer-greater?", 2);
  register_cfun(kernel, pp_integer_less,    "integer-less?", 2);
    
  register_cfun(kernel, pp_cdr, "cdr", 1);
  register_cfun(kernel, pp_car, "car", 1);
  register_cfun(kernel, pp_cons, "cons", 2);


  register_cfun(kernel, pp_quit, "quit", 0);
  register_cfun(kernel, pp_eq, "eq?", 2);
  register_cfun(kernel, pp_pretty_print, "pretty-print", 1);

  register_cfun(kernel, pp_sleep, "sleep", 1);
  register_cfun(kernel, pp_random, "rand", 0);

  
  //defining global variables
  set_global(kernel, makeInt(READ_BUFFER_SIZE), "read-buffer-size");
  set_global(kernel, makeString("0.1", 3), "gt-lisp-version");
  
  return kernel;
}

void init_grapics_bindinds(Scheme_object* env){
  register_cfun(env, g_init, "init-graphics", 0);
  register_cfun(env, g_create_window , "init-window" , 3);
  register_cfun(env, g_swap_buffers , "swap-buff" , 0);
  register_cfun(env, g_terminate, "terminate-window", 0);
  register_cfun(env, g_poll_evnets, "poll-event" ,0);
  register_cfun(env, g_key_is_down_UP, "key-UP", 0);
  register_cfun(env, g_key_is_down_DOWN, "key-DOWN", 0);
  register_cfun(env, g_key_is_down_W, "key-is-down-W", 0);
  register_cfun(env, g_key_is_down_S, "key-is-down-S", 0);

  register_cfun(env, g_draw_rect, "draw-rect", 2);
  register_cfun(env, g_clear, "clear-window", 3);
}

void add_special_macro(char* name, Scheme_object* env, Scheme_object* (*cfunc)(Scheme_object*, Scheme_object*)){
  Scheme_object* symObj = makeSymbolFromCString(name);
  symObj->type = Scheme_MACRO;
  symObj->data.macro.special = true;
  symObj->data.macro.handler.specproc = cfunc;  
  set_global(env,symObj, name);
}

void init_special_macros(Scheme_object*  env){
#define ADD(name, cfun) add_special_macro(name, env, (cfun))
  ADD("if",     eval_if);
  ADD("or",     eval_or);
  ADD("and",    eval_and);
  ADD("define", eval_define); 
  ADD("lambda", eval_lambda); 
  ADD("quoted", eval_quoted);
  ADD("set!",   eval_set);
  ADD("include", eval_include);
  ADD("define-macro", eval_define_macro);
#undef ADD 
}

init_data init(){
  st_init(); //This initializes the global symbol_table.
  Scheme_object* kernel_env = init_kernel_environment();
  init_special_macros(kernel_env);
  Scheme_object* user_global_env = makeEnvironment(kernel_env);

  init_grapics_bindinds(user_global_env);


  su_env_define(kernel_env,
	     makeSymbolFromCString("kernel-global-environment"),
	     kernel_env);

  char* lib_path = "slib/base.scm";
  eval_include(makePair(makeString(lib_path,strlen(lib_path)) ,Scheme_nil), kernel_env);
 
  
  su_env_define(user_global_env,
	     makeSymbolFromCString("user-global-environment"),
	     user_global_env);
  
  
  int bufSize = READ_BUFFER_SIZE;
  string_buffer* buf = malloc(sizeof(string_buffer) + sizeof(char) * bufSize);
  buf->buffer =  (char*)(buf + sizeof(string_buffer));
  buf->size = bufSize;
  buf->index = 0;
  
  return (init_data){
      .env         = user_global_env,
      .readbuffer  = buf,
      };
}
 
