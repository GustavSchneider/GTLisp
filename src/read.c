#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "read.h"
#include "sexp_parser.h"
#include "data/types.h"
#include "symbol_table.h"

#include "data/constructor.h"

#include "util/string.h"
#include "scheme_utils.h"



void insertIntoBuffer(string_buffer* buf, char c){
   
  assert(buf->index < buf->size);
  buf->buffer[buf->index] = c;
  (buf->index)++;
}



void read_input_string(FILE*, string_buffer*);
void read_input_token(FILE*, string_buffer*);
void read_input_list(FILE*, string_buffer*);

void read_input_sexp_string(FILE*, string_buffer*);

void print_error(sexp_parse_result);





char bgetc(FILE* s, string_buffer* b){
  char c = b->extrachar;
  if (c){
    b->extrachar = 0;
    return c;
  } else {
    return getc(s);
  }
  
}

// currently cannot read more bytes then the inital buffer, let it be for now but must change.
Scheme_object* read_sexp(FILE* stream, string_buffer* buf){
  assert(buf != 0);
  assert(buf->size > 0);
  assert(buf->index == 0);
  assert(!feof(stream));

  while(buf->index == 0){
    read_input_sexp_string(stream, buf);
  }
  
  buf->buffer[buf->index] = 0;
  sexp_parse_result sexp_result = sexp_parse(buf->buffer);
  buf->index = 0;
 
  
  if(sexp_result.status != 0){
    return read_sexp(stream, buf);
  } else{
    return sexp_result.scmObj;
  } 
  
}


void throw_away_comment(FILE* stream,string_buffer* buf){
  char c = getc(stream);
  if(c == '\n'){
    buf->extrachar = '\n';
    return;
  } else{
    return throw_away_comment(stream, buf);
  }
}

void read_input_sexp_string(FILE* stream, string_buffer* buf){
  
  char c = bgetc(stream,buf);
  if (c == '\n'){
    return;
  } else if(isWhiteSpace(c)){
    return read_input_sexp_string(stream, buf); 
  } else {
    insertIntoBuffer(buf, c);
    switch(c){
    case '\'':
      return read_input_sexp_string(stream,buf);
    case '(':
      return read_input_list(stream,buf);
    case '"':
      return read_input_string(stream,buf);
    case ';':{
      throw_away_comment(stream,buf);
      return read_input_sexp_string(stream, buf);
    }

    default:
      return read_input_token(stream,buf);
    } 
  }
}


char escaped(char c){
  switch(c){
  case 'n':
    return '\n';
  case 'r':
    return '\r';
  case 'a':
    return '\a';
  case '\\':
    return '\\';
  default:
    printf("Invalid special char %c", c);
    exit(1);
  }

}


void read_input_string(FILE* stream, string_buffer* buf){
  char c = bgetc(stream,buf);
    
  switch(c){
  case '\"':
    insertIntoBuffer(buf, c);
    break;
  case '\\':
    insertIntoBuffer(buf,escaped(getchar()));
    return read_input_string(stream, buf);
    break; 
  default:
    insertIntoBuffer(buf, c);
    return read_input_string(stream, buf);
    break;
  }
}



// Returns bytes read, negative if error
void  read_input_list(FILE* stream, string_buffer* buf){
  int parenBalance = 1;
  char c; 
  while(true){
    c = bgetc(stream,buf);

    if(parenBalance == 0 && isWhiteSpace(c))
      continue;
    
    switch(c){
    case '(':
      parenBalance++;
      break;
    case ')':
      parenBalance--;
      break;
    case '"':{
      
	insertIntoBuffer(buf, c);
	read_input_string(stream, buf);
      }
      break;
    case ';':{
      throw_away_comment(stream,buf);
      continue;
      break;
    }
    }
    
    if(c == '"'){
      continue;
     }
    
    if( stream != stdin && (c == '\n'  || feof(stream))){
      
      if(parenBalance == 0 || feof(stream)){
	return;
      } else {
	continue;
      }
    } else {
      insertIntoBuffer(buf, c);

      if(c == ')' && parenBalance == 0){
	break;
      } else {
	continue;
      }
    }
  }
}

void read_input_token(FILE* stream, string_buffer* buf){
  char c;
  while(true){
    c = bgetc(stream,buf);
    switch(c){
    case '(':
    case ')':
    case '"':
      buf->extrachar = c;
    case ' ':
    case '\t':
    case '\n':
      return;
    default:
      insertIntoBuffer(buf, c);
    }
  }
}




void print_error(sexp_parse_result err_res){
  printf("an error occured\n");
 
};
