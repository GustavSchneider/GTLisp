#include "../src/enviroment.h"
#include "../src/data/types.h"

#include <stdio.h>
#include <assert.h>

void test_things() {
  enviroment* e = newEnviroment(NULL);
  assert(e != NULL);
  int* x = malloc(sizeof(int));
  *x = 10;
  int* y = malloc(sizeof(int));
  *y = 5;
  addSymbol(e, "x", x);
  assert(getSymbol(e, "x") != NULL);
  assert(getSymbol(e, "x") == 10);
  assert(e->size == 10);
  assert(e->items == 1);
  removeSymbol(e, "x");
  assert(getSymbol(e, "x") == NULL);
  assert(e->items == 0);

  addSymbol(e, "x", x);
  addSymbol(e, "y", y);
  printf("%i\n", *getSymbol(e, "x") + *getSymbol(e, "y"));

  deleteEnviroment(e);
}

int main(int argc, char *argv[]) {
  printf("Running tests...\n");
  test_things();
  printf("Ok!\n");
  return 0;
}
