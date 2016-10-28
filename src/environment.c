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

#include "environment.h"
#include "util/string.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "data/prettyprint.h"

#define LOAD_FACTOR(e) ((float) (e)->items) / ((float) (e)->size)
int env_resize(environment* e);

/* Creates a new environment with a map of size INITIAL_SIZE
 */
environment* env_new(Scheme_object* parent) {
  environment* env = malloc(sizeof(environment));
  env->parent = parent;
  env->map = calloc(INITIAL_SIZE, sizeof(bucket*));
  env->items = 0;
  env->size = INITIAL_SIZE;
  return env;
}

/* Adds a symbol to the environment returns 1 if a symbol with the
 * specified name already exists.
 */
int env_define(environment* e, Scheme_symbol* identifier, Scheme_object* p) {

  if (LOAD_FACTOR(e) > RESIZE_THRESHOLD) {
    env_resize(e);
  }

  unsigned long index = identifier->hash % e->size;
  
  while (e->map[index] != NULL) {
    if (e->map[index]->key->hash == identifier->hash) {
      e->map[index]->data = p;
      return 1;
    }
    index++;
    if (index >= e->size) {
      env_resize(e);
      return env_define(e, identifier, p);
    }  
  }
  bucket* new = malloc(sizeof(bucket));
  new->key = identifier;
  new->data = p;
  e->map[index] = new;
  e->items++;

  return 0;
}

/* Returns 1 if symbol exists otherwhise returns 1
 * Note (Warning): the old symbol will be freed.
 */
int env_set(environment* e, Scheme_symbol* identifier, Scheme_object* p) {
  unsigned long index = identifier->hash % e->size;
  while(e->map[index] != NULL && index < e->size) {
    assert(e->map[index]->key != NULL);
    assert(identifier != NULL);
    if (e->map[index]->key->hash == identifier->hash) {
      e->map[index]->data = p;
      return 0;
    }
    index++;
  }
  if (e->parent != NULL)
    return env_set(e->parent->data.env, identifier, p);
  return 1;
}
/* Returns a pointer to the data if found otherwhise returns a null pointer.
 */
Scheme_object* env_lookup(environment* e, Scheme_symbol* identifier) {
  unsigned long index = identifier->hash % e->size;
  for (unsigned long i = index; i < e->size; i++) {
    if (e->map[i] == NULL) {
      if (e->parent == NULL)
	return NULL;
      return env_lookup(e->parent->data.env, identifier);
    }
    if (e->map[i]->key->hash == identifier->hash) {
      return e->map[i]->data;
    }
  }
  
  if (e->parent == NULL) {
    return NULL;
  }
  return env_lookup(e->parent->data.env, identifier);
}


/* Env_Resizes the map by the factor ENV_RESIZE_CONSTANT and frees the old map.
 * because of the way collisions are handled there is a small possibility
 * the size wont be large enough this can lead to multiple env_resizes.
 */
int env_resize(environment* e) {
  bucket** old_map = e->map;
  int old_size = e->size;

  e->size = e->size * RESIZE_CONSTANT;
  e->map = calloc(e->size, sizeof(bucket*));
  e->items = 0;

  for (int i=0; i < old_size; i++) {
    if (old_map[i] != NULL) {
      env_define(e, old_map[i]->key, old_map[i]->data);
      free(old_map[i]);
    }
  }
  free(old_map);
  return 0;
}
