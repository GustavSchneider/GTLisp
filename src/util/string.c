#include <stdbool.h>
#include <stdlib.h>
#include "string.h"

char* dropWhiteSpace(char* str){
  if(! isWhiteSpace(str[0]) || str[0] == '\0'){
    return str; 
  } else {
    return dropWhiteSpace(str + 1);
  } 
};

bool isWhiteSpace(const char c){
  switch(c){
  case ' ':
  case '\t':
  case '\n':
  case '\r':
    return true;
    break;
  default:
    return false;
    break;
  }
}

// return the index of the next whitespace, if none where found return -1
int nextWhiteSpace(const char* str){
    char c = *str;
  if(isWhiteSpace(c)){
    return c;
  } else if(c == '\0'){
    return -1;
  } else {
    return nextWhiteSpace(str + 1);
  }
};
 
// Find the next character of type
int findChar(const char c, const char* str){
  char currentChar = *str;
  if(c == currentChar){
    return c;
  } else if(c == '\0'){
    return -1;
  } else {
    return findChar(c, str + 1);
  }
}
 
bool asciiIsAlpha(char c){
  return 'A' <= c && c <= 'z';
}

bool asciiIsNum(char c){
  return '0' <= c && c <= '9';
}

bool asciiIsLower(char c){
  return 'a' <= c && c <= 'z';
}

bool asciiIsUpper(char c){
  return 'A' <= c && c <= 'Z';
}

void toUpper(char* str, int strLen){
  int i = 0;
  for(; i < strLen; i++){
    char currentChar = str[i];
    if(asciiIsLower(currentChar)){
      str[i] = currentChar + 26;
    }
  }
}

void toLower(char* str, int strLen){
  int i = 0;
  for(; i < strLen; i++){
    char currentChar = str[i];
    if(asciiIsUpper(currentChar)){
      str[i] = currentChar - 26;
    }
  }
}

unsigned long string_hash(char* str, size_t len) {
  unsigned long hash = 5381;
  int c;
  for (int i = 0; i < len; i++) {
    c = (int)str[i];
    hash = ((hash << 5) + hash) + c;
  }
  return hash;
}
