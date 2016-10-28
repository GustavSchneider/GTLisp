#!/bin/bash
gcc -o gt-scheme src/scheme_utils.c src/sexp_parser.c src/data/prettyprint.c src/data/constructor.c src/util/string.c src/symbol_table.c src/data/types.c src/read.c src/eval.c src/apply.c src/repl.c src/environment.c  src/primitive_proc.c src/init.c src/macro.c src/special_functions.c src/garbage_collector.c src/bindings/graphics.c -ggdb -Wall -lGL -lglfw
 
./gt-scheme

