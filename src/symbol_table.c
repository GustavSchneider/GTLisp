/* <GULFTANG Lisp>
 * Copyright (C) <2016>  <Gustav Nelson Schneider>
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
 * <GULFTANG Lisp>  Copyright (C) <2016>  <Gustav Nelson Schneider>
 * This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
 * This is free software, and you are welcome to redistribute it
 * under certain conditions; type `show c' for details.
 */

#include "symbol_table.h"
#include "util/string.h"
#include "data/types.h"
#include <stdlib.h>
#include <stdio.h>

#define LOAD_FACTOR(st) ((float) (st)->items) / ((float) (st)->size)

symbol_table* GLOBAL_SYMBOL_TABLE = NULL;

void st_resize();

void st_init(void) {
  if(GLOBAL_SYMBOL_TABLE == NULL){
  symbol_table* st = malloc(sizeof(symbol_table));
  if (st == NULL) {
    fprintf(stderr, "Failed to allocate memory in st_init!\n");
    exit(1);
  }
  st->symbols = calloc(INITIAL_TABLE_SIZE, sizeof(Scheme_object));
  st->items = 0;
  st->size = INITIAL_TABLE_SIZE;
  GLOBAL_SYMBOL_TABLE = st;
  }
}

Scheme_symbol* st_add(char* name, size_t len) {
  if (LOAD_FACTOR(GLOBAL_SYMBOL_TABLE) > RESIZE_THRESHOLD) {
    st_resize();
  }
  unsigned long hash = string_hash(name, len);
  #ifdef DEBUG
  printf("hash: %ld\n", hash);
  #endif
  unsigned long index = hash % GLOBAL_SYMBOL_TABLE->size;
  while (GLOBAL_SYMBOL_TABLE->symbols[index] != NULL) {
    if (index >= GLOBAL_SYMBOL_TABLE->size) {
      st_resize();
      return st_add(name, len);
    }
    else if (GLOBAL_SYMBOL_TABLE->symbols[index]->hash == hash) {
      return GLOBAL_SYMBOL_TABLE->symbols[index];
    }
    index++;
  }

  Scheme_symbol* new = malloc(sizeof(Scheme_symbol));
  if(new == NULL) {
    fprintf(stderr, "Failed to allocate memory for new symbol\n");
    exit(1);
  }
  new->str.len = len;
  new->str.str = malloc(len + 1);
  if (new->str.str == NULL) {
    fprintf(stderr, "Failed to allocate memory in st_add\n");
    exit(1);
  }
  strncpy(new->str.str, name, len);
  new->str.str[len] = '\0';
  new->hash = hash;
  GLOBAL_SYMBOL_TABLE->symbols[index] = new;
  GLOBAL_SYMBOL_TABLE->items++;
  return new;
}

void st_addStruct(Scheme_symbol* symbol) { 
    if (LOAD_FACTOR(GLOBAL_SYMBOL_TABLE) > RESIZE_THRESHOLD) {
    st_resize();
  }
    
  unsigned long index = symbol->hash % GLOBAL_SYMBOL_TABLE->size;
  while (GLOBAL_SYMBOL_TABLE->symbols[index] != NULL) {
     if (index >= GLOBAL_SYMBOL_TABLE->size) {
      st_resize();
      st_addStruct(symbol);
      return;
    } else if (GLOBAL_SYMBOL_TABLE->symbols[index]->hash == symbol->hash) {
      return;
    }
    index++;
  }
  GLOBAL_SYMBOL_TABLE->symbols[index] = symbol;
  GLOBAL_SYMBOL_TABLE->items++;
  return;
}
/* Do we need this ? */
Scheme_symbol* st_get(unsigned long hash) {
  unsigned long index = hash % GLOBAL_SYMBOL_TABLE->size;
  for (unsigned long i = index; i < GLOBAL_SYMBOL_TABLE->size; i++) {
    if (GLOBAL_SYMBOL_TABLE->symbols[i] == NULL) {
      return NULL;
    }
    if (GLOBAL_SYMBOL_TABLE->symbols[i]->hash == hash) {
      return GLOBAL_SYMBOL_TABLE->symbols[i];
    }
  }
  return NULL;
}

void st_resize() {
  Scheme_symbol** old_table = GLOBAL_SYMBOL_TABLE->symbols;
  int old_size = GLOBAL_SYMBOL_TABLE->size;
  GLOBAL_SYMBOL_TABLE->size = GLOBAL_SYMBOL_TABLE->size * RESIZE_CONSTANT;
  GLOBAL_SYMBOL_TABLE->symbols = calloc(GLOBAL_SYMBOL_TABLE->size, sizeof(Scheme_symbol*));
  GLOBAL_SYMBOL_TABLE->items = 0;
  for (int i=0; i < old_size; i++) {
    if (old_table[i] != NULL) {
      st_addStruct(old_table[i]);
    }
  }
  free(old_table);
}
