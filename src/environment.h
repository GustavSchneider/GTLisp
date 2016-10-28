/* <GULFTANG Lisp>
 * Copyright (C) <2016>  <Gustav Nelson Schneider>
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
 * <GULFTANG Lisp>  Copyright (C) <2016>  <Gustav Nelson Schneider>
 * This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
 * This is free software, and you are welcome to redistribute it
 * under certain conditions; type `show c' for details.
 */

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "data/types.h"

#define INITIAL_SIZE 4
#define RESIZE_THRESHOLD 2.0/3.0
#define RESIZE_CONSTANT 2

 
/* Creates a new environment with all the fields set to 0 */
environment *env_new(Scheme_object* parent);
/* Adds a new value bound to the name specified by name */
int env_define(environment* e, Scheme_symbol* identifier, Scheme_object* p);
/* Basicly updates a value */
int env_set(environment* e, Scheme_symbol* identifier, Scheme_object* p);
/* Find a Scheme_object by the name in the env or its' parents */
Scheme_object* env_lookup(environment* e, Scheme_symbol* identifier);

#endif
 
