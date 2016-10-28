/* <gt-lisp>
 * Copyright (C) <2016>  <Gustav Nelson Schneider & Tomas Möre>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
n * the Free Software Foundation, either version 3 of the License, or
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

#ifndef PRIMITIVE_PROC_H
#define PRIMITIVE_PROC_H
#include "data/types.h"

Scheme_object* pp_is_integer(Scheme_object*);
Scheme_object* pp_is_float(Scheme_object*);
Scheme_object* pp_is_string(Scheme_object*);
Scheme_object* pp_is_bool(Scheme_object*);
Scheme_object* pp_is_proc(Scheme_object*);
Scheme_object* pp_is_symbol(Scheme_object*);
Scheme_object* pp_is_cfun(Scheme_object*);
Scheme_object* pp_is_char(Scheme_object*);
Scheme_object* pp_is_pair(Scheme_object*);
Scheme_object* pp_is_nil(Scheme_object*);

Scheme_object* pp_float_to_integer(Scheme_object*);
Scheme_object* pp_integer_to_float(Scheme_object*);

Scheme_object* pp_range(Scheme_object*, Scheme_object*);
Scheme_object* pp_is_integer_eq(Scheme_object*);
Scheme_object* pp_integer_less(Scheme_object*);
Scheme_object* pp_integer_greater(Scheme_object*);

Scheme_object* pp_integer_add(Scheme_object*);
Scheme_object* pp_integer_sub(Scheme_object*);
Scheme_object* pp_integer_mult(Scheme_object*);
Scheme_object* pp_integer_div(Scheme_object*);
Scheme_object* pp_mod(Scheme_object*);

Scheme_object* pp_car(Scheme_object*);
Scheme_object* pp_cdr(Scheme_object*);

Scheme_object* pp_cons(Scheme_object*);
Scheme_object* pp_quit(Scheme_object*);
Scheme_object* pp_pretty_print(Scheme_object*);
Scheme_object* pp_eq(Scheme_object*);


Scheme_object* pp_string_to_symbol(Scheme_object*);
Scheme_object* pp_symbol_to_stringl(Scheme_object*);

Scheme_object* pp_sleep(Scheme_object*);
Scheme_object* pp_random(Scheme_object*);

#endif /*PRIMITIVE_PROC_H*/
