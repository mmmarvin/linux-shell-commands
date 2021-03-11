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
#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include <stdbool.h>
#include <stddef.h>
#include "path.h"

typedef struct
{
  bool    permanent;
  bool    quiet;
  path_t* files;
  bool*   files_deleted;
  size_t  num_files;
} run_param_t;

void init_run_param(run_param_t* param);
void destroy_run_param(run_param_t* param);

bool parse_flags(const char* flags, run_param_t* param);
bool parse_files(const char* flags, run_param_t* param, size_t current_file_index);

#endif // ARG_PARSER_H
