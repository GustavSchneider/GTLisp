#include "types.h"
#include "prettyprint.h"
#include "constructor.h"
#include <stdio.h>
#include <stdbool.h>

void printList(Scheme_object*);
void printPair(Scheme_object*);

bool isList(Scheme_object* pair){
  Scheme_type cdrType = pair->data.pair.cdr->type;
  bool thisIsPair = pair->type == Scheme_PAIR;
  bool nextIsPair =  Scheme_PAIR == cdrType;
  bool isNil = cdrType == Scheme_NIL;
  return thisIsPair && ( nextIsPair || isNil );
}


void printPair(Scheme_object* scmObj){
  printf("(");
  prettyprint(scmObj->data.pair.car);
  printf(" . ");
  prettyprint(scmObj->data.pair.cdr);
  printf(")");
}
void printList(Scheme_object* listStart){
  printf("(");
  Scheme_object* currentObj = listStart;

  bool goOn = true;
  while(goOn){
      prettyprint(currentObj->data.pair.car);
      Scheme_object* next  = currentObj->data.pair.cdr;
      
      if(next->type != Scheme_NIL){
	printf(" ");
      }
      
      currentObj = next;

      switch(currentObj->type){
      case Scheme_PAIR:
	break;
      case Scheme_NIL:
	goOn = false;
	break;
      default:
	goOn = false;
	printf(". ");
	prettyprint(currentObj);
	break;
      }
  }
  
  printf(")");
  
}

void prettyprint(Scheme_object* scmObj){
  
  Scheme_type scmtype  = scmObj->type;
  
  switch(scmtype){
  case Scheme_NIL:
    printf("()"); 
    break;
  case Scheme_PAIR:
    if(isList(scmObj)){
      printList(scmObj);
    } else{
      printPair(scmObj);
    }
    break;
  case Scheme_STRING:
    printf("\"");
    fwrite(scmObj->data.string.str,1,(int)scmObj->data.string.len,stdout);
    printf("\"");
    //printf("%.s", (int)scmObj->data.string.length, scmObj->data.string.charPtr);
    break;
  case Scheme_INTEGER:
    printf("%ld",scmObj->data.integer);
    break;
  case Scheme_FLOAT:
    printf("%f", scmObj->data.real);
    break;
  case Scheme_SYMBOL:
    printf("%s", scmObj->data.symbol->str.str);
    break;
  case Scheme_BOOL:
    printf("#%s", scmObj == Scheme_false ? "f":"t");
    break;
  case Scheme_VOID:
    break;
  default:
    printf("[UNPRINTABLE F*KING OBJECT %d]", scmtype);
    break;
  }
}



