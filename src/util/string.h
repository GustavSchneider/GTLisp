#ifndef UTILSTRING_H
#define UTILSTRING_H

#include <stdbool.h>
#include <stdlib.h>

char* dropWhiteSpace(char*);


// returns true if the char is 
bool isWhiteSpace(const char);

int nextWhiteSpace(const char*);

int findChar(const char, const char*);


bool asciiIsAlpha(char);

bool asciiIsNum(char);

bool asciiIsUpper(char);

bool asciiIsLower(char);


void asciiToLower(char*, int);
void asciiToUpper(char*, int);

unsigned long string_hash(char *str, size_t len);
#endif
