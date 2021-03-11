/**********
 *
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <https://www.gnu.org/licenses/>. 
 *
 **********/
#ifndef PATH_H
#define PATH_H

#include <stdbool.h>
#include <stddef.h>
#include <limits.h>

typedef struct
{
  char value[PATH_MAX + 1];
} path_t;

size_t path_len(const path_t* p);
const char* get_path_str(const path_t* p);

bool is_dir(const path_t* p);

path_t create_path(const char* p);
path_t copy_path(const path_t* p);

path_t combine_path(const path_t* p1, const path_t* p2);
path_t combine_path_string(const path_t* p1, const char* p2);

path_t remove_extension(const path_t* p);

path_t get_root_path(const path_t* p);
path_t get_filename(const path_t* p);
path_t get_file_extension(const path_t* p);

#endif // PATH_H
