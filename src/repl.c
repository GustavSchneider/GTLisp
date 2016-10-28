/* <gt-lisp>
 * Copyright (C) <2016>  <Gustav Nelson Schneider & Tomas Möre>
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
 * <gt-lisp> Copyright (C) <2016>  <Gustav Nelson Schneider & Tomas Möre>
 * This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
 * This is free software, and you are welcome to redistribute it
 * under certain conditions; type `show c' for details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "sexp_parser.h"
#include "data/types.h"
#include "data/prettyprint.h"
#include "read.h"
#include "environment.h"
#include "eval.h"
#include "apply.h"
#include "primitive_proc.h" 
#include "data/constructor.h"
#include "init.h"
#include "garbage_collector.h"

#define PROMPT_STRING "GT >> "


void prompt(bool newline){
  printf(newline ? "\n%s": "%s", PROMPT_STRING);
}

void print_header() {
  char *path = "misc/ascii";
  FILE* fp = fopen(path, "r");
  if (!fp) {
    fprintf(stderr, "Failed to open file %s\n", path);
    exit(1);
  }
  
  int ret = fseek(fp, 0L, SEEK_END);
  if (ret) {
    fprintf(stderr, "Failed to determine file size of %s\n", path);
  }
  
  int size = ftell(fp);
  if (size == -1) {
    fprintf(stderr, "Failed to determine file size of %s\n", path);
  }
  
  rewind(fp);
  char* header = calloc(size + 1, sizeof(char));
  ret = fread(header, size, sizeof(char), fp);
  printf("%s", header);
  fclose(fp);
  free(header);
  
  printf("Welcome to gt-scheme! Type something fun under here. Pray it doesn't crash\n");
}


int main(int argc, char *argv[]) {
  init_data init_data = init();
  
  print_header();
  bool newline = false;
  
  while(true){
    prompt(newline);
    Scheme_object* code = read_sexp(stdin, init_data.readbuffer);

    Scheme_object* result = eval(code, init_data.env, NULL); 

    newline = result != Scheme_void; 
    prettyprint(result);
    GB_RUN(init_data.env);
  }
  
  return 0;
}










