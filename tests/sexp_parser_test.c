#include "../src/data/types.h"
#include "../src/sexp_parser.h"
#include "../src/data/prettyprint.h"
#include "../src/symbol_table.h"
#include <stdio.h>

#include "../src/util/dbg.h"




int main(int argc, char** argv){
  if(argc > 1){

    DBG("Testing!");
    
    printf("test input: %s\n", argv[1]);
    char* testString = argv[1];
    symbol_table* st = st_new_table();
    sexp_parse_result sexp_res = sexp_parse(testString, st);
    if(sexp_res.status ==  SEXP_PARSE_SUCCESS){
      
      prettyprint(sexp_res.scmObj);
    } else {
      printf("Parsing failed with error code: %d\n",sexp_res.status);
    }
    return 0;
  }
  printf("no args given");
  
}
 
