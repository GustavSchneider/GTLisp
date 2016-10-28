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

#ifndef DEFINES_H
#define DEFINES_H

#define O_RDONLY   00000000
#define O_WRONLY   00000001
#define O_RDWR     00000002
#define O_CREAT    00000100
#define O_APPEND   00002000

#define S_IRUSR    00400 //user has read permission
#define S_IRWXU    00700 //user has read, write and execution permission
#define S_IWUSR    00200 //user has write permission
#define S_IRWXG    00070 //group has read, write and execute permission
#define S_IRGRP    00040 //roup has read permission
#define S_IWGRP    00020 //roup has write permission
#define S_IXGRP    00010 //roup has execute permission
#define S_IRWXO    00007 //others have read, write and execute permission
#define S_IROTH    00004 //others have read permission
#define S_IWOTH    00002 //others have write permission
#define S_IXOTH    00001 //others have execute permission
#endif
