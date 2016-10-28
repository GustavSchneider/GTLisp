#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "data/types.h"
#include "garbage_collector.h"
#include "data/prettyprint.h"

garbage_node* GARBAGE_LIST = NULL;
int OBJECT_ON_HEAP = 0;

void gb_collect(Scheme_object*);
void gb_sweep();
void gb_mark(Scheme_object*);
void GB_REGISTER(Scheme_object*);

void GB_REGISTER(Scheme_object* o) {
  garbage_node* new = malloc(sizeof(garbage_node));
  new->next = GARBAGE_LIST;
  new->data = o;
  GARBAGE_LIST = new;
  OBJECT_ON_HEAP++;
}

void gb_mark(Scheme_object* head) {
  if (head == NULL || head->marked) {
    return;
  }
  
  head->marked = 1;
  
  switch (head->type) {
  case Scheme_ENV: {
    environment* env = head->data.env;
    if (env->parent != NULL) {
      gb_mark(env->parent);
    }
    for (unsigned long i = 0; i < env->size; i++) {
      if (env->map[i] != NULL) {
	gb_mark(env->map[i]->data);
      }
    }
    break; 
  }
  case Scheme_PAIR: {
    gb_mark(head->data.pair.cdr);
    gb_mark(head->data.pair.car);
    break;
  }
  case Scheme_EXCEPTION: {
    gb_mark(head->data.exception.type);
    gb_mark(head->data.exception.msg);
    break;
  }
  case Scheme_PROCEDURE: {
    gb_mark(head->data.proc.scope);
    gb_mark(head->data.proc.args);
    gb_mark(head->data.proc.body);
    break;
  }
  case Scheme_MACRO:{
    if(!head->data.macro.special){
      gb_mark(head->data.macro.handler.scmproc.args);
      gb_mark(head->data.macro.handler.scmproc.scope);
      gb_mark(head->data.macro.handler.scmproc.body);
    }
    break;
  }
  default: {}
  }
}

void gb_sweep() {
  int count = 0;

  garbage_node* prev = NULL;
  garbage_node* current = GARBAGE_LIST;
  while (current != NULL) {
    Scheme_object* data = current->data;
    if (data->marked) {
      data->marked = 0;
      prev = current;
      current = current->next;
    } else {
      count++;
      //prettyprint(data);
      //puts("");
      gb_collect(data);
      //puts("done");
      if (prev != NULL) {
	prev->next = current->next;
      } else {
	GARBAGE_LIST = current->next;
      }
      garbage_node* tmp = current;
      current = current->next;
      
      assert(tmp != NULL);
      free(tmp);
    } 
  }
  int old = OBJECT_ON_HEAP;
  OBJECT_ON_HEAP -= count;
}

void gb_collect(Scheme_object* o) {
  assert(o != NULL);
  
  switch (o->type) {
  case Scheme_ENV: {
   
    environment* env = o->data.env;
    for (unsigned long i = 0; i < env->size; i++) {
      if (env->map[i]) {
	free(env->map[i]);
      }
    }
    free(env->map);
    free(env);
    free(o);
    break;
  }
  case Scheme_STRING: {
    free(o->data.string.str);
    free(o);
    break;
  }
  case Scheme_SYMBOL:
  case Scheme_INTEGER:
  case Scheme_FLOAT:
  case Scheme_MACRO:
  case Scheme_EXCEPTION:
  case Scheme_PAIR:
  case Scheme_CFUNC:
  case Scheme_PROCEDURE: {
    if (o)
      free(o);
    break;
  }
  default: {}
  }
}

void GB_RUN(Scheme_object* head) {
  if(OBJECT_ON_HEAP > 300){
    gb_mark(head);
    gb_sweep();
  }
}
