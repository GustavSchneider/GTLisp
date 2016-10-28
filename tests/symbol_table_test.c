#include "../src/symbol_table.h"
#include "../src/data/types.h"
#include "../src/util/string.h"

#include <assert.h>
#include <stdio.h>

void monster(void) {
  symbol_table* st = st_new_table();
  assert(st->items == 0);
  assert(st->size == INITIAL_TABLE_SIZE);
  st_add(st, "aaa", 3);
  st_add(st, "aab", 3);
  st_add(st, "hellowo", 7);
  st_add(st, "hellowooow", 10);
  st_add(st, "aac", 3);

  assert(st->items == 5);
}

int main(void) {
  printf("Running tests for symbol_table.c\n");
  monster();
  printf("Ok!\n");
  return 0;
}
