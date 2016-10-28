#include <stdio.h>
#include <assert.h>

#include "../src/read.h"
#include "../src/symbol_table.h"
#include "../src/data/prettyprint.h"







int main(){
  FILE* fp = fopen("data/readtestdata.txt","r");
  assert(fp != 0);
  
  symbol_table* st = st_new_table();

  while(true){
  Scheme_object* fstRes = read_sexp(stdin, buf, bufSize, st);
  prettyprint(fstRes);
  }
  

  return 0;

}
