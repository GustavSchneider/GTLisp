

#include "data/types.h"
#include "util/string.h"
#include "util/dbg.h"
#include "data/constructor.h"

#include "sexp_parser.h"
#include "util/string.h"
#include "symbol_table.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>




typedef enum{
  SEXP_UNKNOWN,
  SEXP_LIST,
  SEXP_SYMBOL,
  SEXP_DOT,
  SEXP_STRING,
  SEXP_INTEGER,
  SEXP_FLOAT,
  SEXP_QUOTED,
  SEXP_KEYWORD
} sexp_atom_type;
 

typedef struct{
  char* start;
  char* end;
} string_slice;


#define slice_length(slice) ((slice).end - (slice).start)

bool atom_slice_valid(string_slice s){
  return s.start < s.end;
}

string_slice slice_dropWhiteSpace(string_slice s){
  while(isWhiteSpace(*s.start) && s.start < s.end){
    s.start++;
  }
  return s;
}

#define PM_ERROR(e) (parse_monad) {.status = e, .data.ptr = 0}
#define PM_NULL (parse_monad) { .status = 0, .data.ptr = 0};
#define PM_RETURN(t, d) (parse_monad) { .status = 0, .data.t = d};


// Yeah I just did that I'm a filthy functional programmer
typedef struct {
  sexp_parse_status status;
  union{
    string_slice   slice;
    Scheme_object* sObj;
    void*          ptr;
    sexp_atom_type atomType;
  } data;
} parse_monad;

#define pmSuccess(pm) pm.status == 0
#define pmError(pm) !pmSuccess(pm)
 



/*
  function to finds the next sexp from a given start char*.
  returns a string slice.
 */
parse_monad sexp_find_atom(string_slice);
parse_monad sexp_next_atom(string_slice, string_slice);

/*
  finds the character representing the end of an sexp 
  returns 0 if end was reached
 */
parse_monad findAtomEnd(string_slice);

parse_monad findEndOfToken(string_slice);
parse_monad findStringEnd(string_slice);
parse_monad findQuotedAtomEnd(string_slice);
parse_monad findMatchingParen(string_slice);



parse_monad sexp_parse_slice(string_slice, sexp_atom_type);
parse_monad sexp_parse_list(string_slice);

parse_monad parse_keyword(string_slice);

/*
  idenified the type of the sexp, this cannot handle dots 
  the first character must not be a white space
  the data field is the sexp_atom_type
*/

sexp_atom_type sexp_identify(string_slice);
sexp_atom_type identitfyToken(string_slice);

sexp_parse_result sexp_parse(char* input_str){
  sexp_parse_result result;
  
  DBG("sexp_parse recieved string: %s\n", input_str);

    
  string_slice slice = {.start = input_str, .end = input_str};

  while(*slice.end != '\0'){
    slice.end++;
  }
  
  DBG("InputString:\n \t start: %lu\n\t end:   %lu\n\t length: %lu", slice.start, slice.end, slice_length(slice));
  
  parse_monad pmRes = sexp_parse_slice(slice, SEXP_UNKNOWN);
  DBG("Sexp parsed!");

  
  DBG("sexp_parse result status: %u", pmRes.status);
  
  
  result.status = pmRes.status;
  
  if(pmRes.status == 0){
    result.scmObj = pmRes.data.sObj;
  }
  
  DBG("Returning stuff");

  return result;
  
}


parse_monad sexp_parse_slice(string_slice inSlice,  sexp_atom_type inputType){

  string_slice s = slice_dropWhiteSpace(inSlice);
  parse_monad result = PM_NULL;

  //assert(atom_slice_valid(s));
  if(!atom_slice_valid(s))
    return PM_RETURN(sObj, Scheme_nil);
  
  parse_monad atomSliceM;
  string_slice atomSlice;
  
  if(inputType == SEXP_UNKNOWN){
    atomSliceM  = sexp_find_atom(s);
    DBG("Atom parse monad flag: %d", atomSliceM.status);
  
    
    if(atomSliceM.status == SEXP_PARSE_ERROR_DATUM_EXPECTED){
      DBG("Decided that atom was nil");
      
      result.status = 0;
      result.data.sObj = Scheme_nil;
      return result;
    }

    assert(pmSuccess(atomSliceM));
  
    atomSlice = atomSliceM.data.slice;
  } else {
    atomSlice = s;
  }
  
  DBG("Atom found length: %d, \"%.*s\"", slice_length(atomSlice), slice_length(atomSlice), atomSlice.start);

  
  sexp_atom_type atomType;
  if(inputType == SEXP_UNKNOWN){
    atomType = sexp_identify(atomSlice);
  } else {
    atomType = inputType;
  }
  
  DBG("Atom type derived to be %d", atomType);
 

  // Handling Dots
  switch(atomType){
  case SEXP_DOT:
    result = PM_ERROR(SEXP_PARSE_ERROR_IMPROPERLY_PLACED_DOT);
    break;
  case SEXP_SYMBOL:
    result = PM_RETURN(sObj, makeSymbol(atomSlice.start, slice_length(atomSlice)));
    break;
    
  case SEXP_LIST:
    atomSlice.start++;
    DBG("Starting to parse list");
    result = sexp_parse_list(atomSlice);
    break;
  case SEXP_STRING:
    result = PM_RETURN(sObj, makeString(atomSlice.start + 1, slice_length(atomSlice) - 1));
    break;
  case SEXP_QUOTED:{
    atomSlice.start++;
    atomSlice.end++;
    parse_monad quotedObjM = sexp_parse_slice(atomSlice,SEXP_UNKNOWN);
    if(pmError(quotedObjM)){
      result = quotedObjM;
    } else{
      
      result = PM_RETURN(sObj, makeQuoted(quotedObjM.data.sObj));
    }
    break;
  }
  case  SEXP_KEYWORD:
    result = parse_keyword(s);
    break;
  default:
    {
      // Using c's build in parsing methods therefore i need a c string.
      int cStrLen = slice_length(atomSlice) + 1;
      char* cStr = malloc(cStrLen * sizeof(char));
      cStr[cStrLen - 1] = '\0';
      strncpy(cStr, atomSlice.start, slice_length(atomSlice));
      switch(atomType){
      case SEXP_INTEGER:
	result = PM_RETURN(sObj, makeInt(atoi(cStr)));
	break;
      case SEXP_FLOAT:
	result = PM_RETURN(sObj, makeFloat(atof(cStr)));
	break;
      default:
	assert(0 && "WTF HAPPEND HERE?!");
	
      }
      free(cStr);
    }
  }

  return result;
  
}



parse_monad sexp_parse_list(string_slice inSlice){

  string_slice s = slice_dropWhiteSpace(inSlice);
  if(slice_length(s) == 0){
    DBG("End of list reached, returning Nil");
 
    return PM_RETURN(sObj,Scheme_nil);
  } else {
    parse_monad atomSliceM = sexp_find_atom(s);
    if(pmError(atomSliceM)){
      return atomSliceM;
    }
    string_slice atomSlice = atomSliceM.data.slice;
    sexp_atom_type atomType = sexp_identify(atomSlice);
 
    if(atomType == SEXP_DOT){
  
      DBG("Encountered a dot in list\n");
      
      parse_monad nextAtomSliceM = sexp_next_atom(s, atomSlice);
      if(pmError(nextAtomSliceM)){
	return PM_ERROR(SEXP_PARSE_ERROR_IMPROPERLY_PLACED_DOT);
      }
      string_slice nextAtomSlice = nextAtomSliceM.data.slice;

      string_slice remaningSlice = slice_dropWhiteSpace((string_slice){
	  .start = nextAtomSlice.end,
	    .end = inSlice.end
	    });
      if(remaningSlice.start != remaningSlice.end){
	DBG("Encountered an error in dot parsing"); 
  
	return PM_ERROR(SEXP_PARSE_ERROR_INCOMPLETE_FORM);
      } else{

	DBG("Dot parsing success");

	return sexp_parse_slice(nextAtomSlice,  SEXP_UNKNOWN);
      }

    } else {
      parse_monad sObjM = sexp_parse_slice(atomSlice,  atomType);

      if(pmError(sObjM)){
	return sObjM;
      }
      Scheme_object* pair = makeEmptyPair();
      pair->data.pair.car = sObjM.data.sObj;
      char* nextStart = atomSlice.end == s.end ? s.end : atomSlice.end + 1;
      parse_monad cdrSObjM = sexp_parse_list(((string_slice){ .start = nextStart , .end = s.end}));
      if(pmError(cdrSObjM))
	return cdrSObjM;
      pair->data.pair.cdr = cdrSObjM.data.sObj;
      return PM_RETURN(sObj, pair);
    }
  }
}

inline parse_monad sexp_next_atom( string_slice boundary, string_slice prevAtom )
{
  return sexp_find_atom((string_slice){ .start = prevAtom.end, . end = boundary.end});
}

parse_monad sexp_find_atom(string_slice s){

  DBG("trying to find atom bounds %lu, %lu: \"%.*s\"", s.start, s.end, slice_length(s), s.start);

#ifdef  DEBUG
  char* origStart = s.start;
#endif
  
  assert(atom_slice_valid(s));
  assert(slice_length(s) > 0);
  
  parse_monad result = PM_NULL;
  while(true){
    if(!atom_slice_valid(s)){
      DBG("Failed to find atom within bounds");

      result = PM_ERROR(SEXP_PARSE_ERROR_DATUM_EXPECTED);
      break;
    } else if(isWhiteSpace(*s.start)){
      s.start++;
    } else {
      result = findAtomEnd(s);
      break;
    }
  }

#ifdef DEBUG
  if(result.status != 0){
    printf("Failed to find atom error %d\n", result.status);
  }
#endif 


  DBG("Atom bounds within from %lu to %lu %.*s", result.data.slice.start,
	 result.data.slice.end, slice_length(result.data.slice),result.data.slice.start);

  assert(result.status == 0);
  return result;
 
}


parse_monad findAtomEnd(string_slice s){
  assert(slice_length(s) > 0);
  DBG("trying to find end of atom bounds %lu, %lu: \"%.*s\"",
	   s.start, s.end, slice_length(s), s.start);

  parse_monad result;
  
  switch(*s.start){
  case '\"':
    DBG("Atom is string");
    result = findStringEnd(s);
    break;
  case '\'':
    DBG("Atom is quoted");
    result = findQuotedAtomEnd(s);
    break;
  case '(':
    DBG("Atom is list / pair");
    result = findMatchingParen(s);
    break;
  default:
    DBG("Atom is a token");
    result = findEndOfToken(s);
    break;
  }
  
  return result;
}




sexp_atom_type sexp_identify(string_slice s){
  assert(atom_slice_valid(s) && "The slice isn't valid");
  assert(!isWhiteSpace(*s.start) && "The start of a symbol may not be white space");

  sexp_atom_type type = SEXP_SYMBOL;
  char firstChar = *s.start;
  switch(firstChar){
  case '\"':
    type = SEXP_STRING;
    break;
  case '(':
    type = SEXP_LIST;
    break;
  case '\'':
    type = SEXP_QUOTED;
    break;
  case '#':
    type = SEXP_KEYWORD;
    break;
  default:
    if(firstChar == '.' && ( s.start + 1 >= s.end || isWhiteSpace(*(s.start + 1)))){
      type = SEXP_DOT;
    } else {
      type = identitfyToken(s);
    }   
  }
  DBG("atom is of sexp_atom_type %d", type);
  
  return type;
}

/*
  Indetifies a token. Shouldn't be able to fail
*/
sexp_atom_type identitfyToken(string_slice s){
  assert(atom_slice_valid(s));
  assert(!isWhiteSpace(*s.start));
  
  sexp_atom_type type = SEXP_SYMBOL;
  int i;
  for(i = 0; i < slice_length(s); i++){
    char c = s.start[i];
    
    if(asciiIsAlpha(c)){
      type = SEXP_SYMBOL;
      break;
    } if(c == '.'){
      
      if(type == SEXP_FLOAT){
	type = SEXP_SYMBOL;
	break;
      } else {
	type = SEXP_FLOAT;
      }
      
    }  else if (asciiIsNum(c) && type != SEXP_FLOAT){
	type = SEXP_INTEGER;
    } else if( c == '-'){
	// skip
    } 
  }

  DBG("\"%.*s\" token got identified as %d", slice_length(s), s.start, type);
  return type;
}

parse_monad findQuotedAtomEnd(string_slice s){
  parse_monad result = PM_NULL;
  char* orig_start = s.start;
  if(isWhiteSpace(*(s.start + 1))){
    result =  PM_ERROR(SEXP_PARSE_ERROR_DATUM_EXPECTED );
  } else {
    s.start++;
    result = findAtomEnd(s);
    if(pmSuccess(result))
      result.data.slice.start = orig_start;
  }
  return result;
}

/* Returns the adress of the matching paren, null if there was an error.
   The functions expects the first char to be an '('
*/
parse_monad findMatchingParen(string_slice s){

  char* maxEnd = s.end;
  int balance = 1;
  
  parse_monad result = PM_NULL;    
  for(s.end = s.start + 1; s.end < maxEnd; s.end++){
    
    switch(*s.end){
    case '(':
      balance++;
      break;
    case ')':
      balance--;
      break; 
    }
    
    if(balance == 0){
      break;
    } 
  }


  if(balance != 0){
    result = PM_ERROR(SEXP_PARSE_ERROR_DATUM_EXPECTED);
  } else {

    
    result.data.slice = s;
  }

  return result;
}


/*
  Finds the end of a string, these are identifed by a double quote (")
  Ignores any escaped double quotes, the escaping char is a backslash '\'

  returns a slice
 */
parse_monad findStringEnd(string_slice s){
  DBG("Trying to find end of string atom of %.*s", slice_length(s), s.start);
  
  parse_monad result =  PM_NULL;
  char* maxEnd = s.end;
  s.end = s.start + 1;
  
  while(s.end < maxEnd){
    
    switch(*s.end){
    case '\\': 
      s.end += 2; // this means skip the next char
      break;
    case '\"':

      DBG("Found end of string at %lu", s.end);

      result.data.slice = s;
      return result;
      break;
    default:
      s.end++;
      break;
    }
  }
  DBG("Found string: (%.*s)", slice_length(s), s.start);
  

  if(result.data.ptr == 0){
    result = PM_ERROR(SEXP_PARSE_ERROR_DATUM_EXPECTED);
  }
  
  return result;
}




bool is_token_ending_char(char c){
  switch(c){
  case '\n':
  case ' ':
  case '\t':
  case '(':
  case ')':
  case '\'':
  case '"':
  case '\0':
    return true;
  default:
    return isWhiteSpace(c);
  }
}
/*
  Returns the end of a token, these are identified by a whiespace or null
  The end of the slice sent in is expected to be the max value of the bounding slice;
 */
parse_monad findEndOfToken(string_slice s){
  assert(slice_length(s) > 0);
  assert(!isWhiteSpace(*s.start));

  DBG("Trying to find end of token:\n");
    
  char* maxEnd = s.end;
  parse_monad result = PM_NULL;
  s.end = s.start;
  while(!is_token_ending_char(*s.end) && s.end < maxEnd){
 
    s.end++;
  }

#ifdef DEBUG
  if(pmError(result)){
    printf("Error in finding end last char is: %c", *(s.end - 1));
  } else{
    printf("Found token! length: %d", s.end - s.start);
  }
#endif
  
  if(result.status == 0){
    result.data.slice = s;
  }
  return result;
}



// This functioncurrently sucks, change it
parse_monad parse_keyword(string_slice s){
#define ISKEYWORD(instr) (strncmp((instr), keyword.start, slice_length(keyword)) == 0)
  parse_monad result;
  string_slice keyword = s;
  keyword.start ++;
  
  if(slice_length(keyword) <= 0){
    result = PM_ERROR(SEXP_PARSE_ERROR_DATUM_EXPECTED);
  } else if (ISKEYWORD("t")){
    result = PM_RETURN(sObj,Scheme_true);
  } else if (ISKEYWORD("f")){
    result = PM_RETURN(sObj, Scheme_false);
  } else if (ISKEYWORD("void")){
    result = PM_RETURN(sObj, Scheme_void);
  } else {
    fprintf(stderr, "Invalid keyword %.*s\n", (int)slice_length(keyword), keyword.start);
    exit(1336);
  } 


  return result;
#undef ISKEYWORD
}
