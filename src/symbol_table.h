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
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "data/types.h"

#define INITIAL_TABLE_SIZE 100
#define RESIZE_THRESHOLD 2.0/3.0
#define RESIZE_CONSTANT 2

typedef struct {
  Scheme_symbol** symbols;
  unsigned long items;
  unsigned long size;
} symbol_table;

void st_init();

Scheme_symbol* st_add( char* name, size_t len);
/*Returns the symbol if exits otherwhise NULL*/
Scheme_symbol* st_get( unsigned long hash);

#endif /*  SYMBOL_TABLE_H */
